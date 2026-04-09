#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

using namespace std;

namespace avl {

template <typename Key, typename Value>
class AVL {
   private:
    struct Node {
        Key key;
        Value value;
        int height = 1;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(const Key& k, const Value& v) : key(k), value(v) {}
    };

    Node* root_ = nullptr;
    size_t size_ = 0;

    int get_height(Node* node) const { return node ? node->height : 0; }

    int balance_factor(Node* node) const {
        return node ? get_height(node->right) - get_height(node->left) : 0;
    }

    void fix_height(Node* node) {
        if (!node) return;
        int hl = get_height(node->left);
        int hr = get_height(node->right);
        node->height = (hl > hr ? hl : hr) + 1;
    }

    Node* rotate_right(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        fix_height(p);
        fix_height(q);
        return q;
    }

    Node* rotate_left(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fix_height(q);
        fix_height(p);
        return p;
    }

    Node* balance(Node* p) {
        if (!p) return nullptr;
        fix_height(p);

        if (balance_factor(p) == 2) {
            if (balance_factor(p->right) < 0) p->right = rotate_right(p->right);
            return rotate_left(p);
        }
        if (balance_factor(p) == -2) {
            if (balance_factor(p->left) > 0) p->left = rotate_left(p->left);
            return rotate_right(p);
        }
        return p;
    }

    Node* insert(Node* node, const Key& key, const Value& value,
                 bool& success) {
        if (!node) {
            success = true;
            ++size_;
            return new Node(key, value);
        }

        if (key < node->key)
            node->left = insert(node->left, key, value, success);
        else if (key > node->key)
            node->right = insert(node->right, key, value, success);
        else
            success = false;

        return balance(node);
    }

    Node* remove_min(Node* node, Node*& min_node) {
        if (!node->left) {
            min_node = node;
            return node->right;
        }
        node->left = remove_min(node->left, min_node);
        return balance(node);
    }

    Node* remove(Node* node, const Key& key, bool& success) {
        if (!node) {
            success = false;
            return nullptr;
        }

        if (key < node->key)
            node->left = remove(node->left, key, success);
        else if (key > node->key)
            node->right = remove(node->right, key, success);
        else {
            success = true;
            --size_;

            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;

            } else {
                Node* min_node = nullptr;
                node->right = remove_min(node->right, min_node);

                min_node->left = node->left;
                min_node->right = node->right;

                delete node;
                return balance(min_node);
            }
        }
        return balance(node);
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    void serialize(Node* node, std::ofstream& out) const {
        if (!node) return;

        uint16_t len = static_cast<uint16_t>(node->key.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(node->key.data(), len);
        out.write(reinterpret_cast<const char*>(&node->value),
                  sizeof(node->value));

        serialize(node->left, out);
        serialize(node->right, out);
    }

   public:
    AVL() = default;

    ~AVL() { clear(); }

    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    AVL(AVL&& other) noexcept : root_(other.root_), size_(other.size_) {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    AVL& operator=(AVL&& other) noexcept {
        if (this != &other) {
            clear();
            root_ = other.root_;
            size_ = other.size_;
            other.root_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    bool insert(const Key& key, const Value& value) {
        bool success = false;
        root_ = insert(root_, key, value, success);
        return success;
    }

    bool remove(const Key& key) {
        bool success = false;
        root_ = remove(root_, key, success);
        return success;
    }

    std::tuple<bool, Value> find(const Key& key) const {
        Node* node = root_;
        while (node) {
            if (key == node->key) return {true, node->value};
            node = (key < node->key) ? node->left : node->right;
        }
        return {false, Value()};
    }

    void clear() {
        destroy(root_);
        root_ = nullptr;
        size_ = 0;
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    bool save(const std::string& path) const {
        std::ofstream out(path, std::ios::binary | std::ios::trunc);

        const char magic[8] = "AVLDICT";
        out.write(magic, 8);

        uint64_t current_size = size_;
        out.write(reinterpret_cast<const char*>(&current_size),
                  sizeof(current_size));

        serialize(root_, out);

        return out.good();
    }

    bool load(const std::string& path) {
        std::ifstream in(path, std::ios::binary);

        in.seekg(0, std::ios::end);
        if (in.tellg() == 0) {
            clear();
            return true;
        }
        in.seekg(0, std::ios::beg);

        char magic[8];
        if (!in.read(magic, 8) || std::memcmp(magic, "AVLDICT", 8) != 0)
            return false;

        uint64_t new_size;
        if (!in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size)))
            return false;

        AVL<Key, Value> temp_tree;

        for (uint64_t i = 0; i < new_size; ++i) {
            uint16_t len;
            if (!in.read(reinterpret_cast<char*>(&len), sizeof(len)))
                return false;

            Key key(len, '\0');
            if (!in.read(&key[0], len)) return false;

            Value value;
            if (!in.read(reinterpret_cast<char*>(&value), sizeof(value)))
                return false;

            temp_tree.insert(key, value);
        }

        *this = std::move(temp_tree);
        return true;
    }
};
}

void to_lower(std::string& str) {
    for (char& c : str) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    avl::AVL<std::string, uint64_t> dictionary;
    std::string command;

    while (std::cin >> command) {
        try {
            if (command == "+") {
                std::string word;
                uint64_t value;
                std::cin >> word >> value;
                to_lower(word);

                if (dictionary.insert(word, value)) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "Exist\n";
                }
            } else if (command == "-") {
                std::string word;
                std::cin >> word;
                to_lower(word);

                if (dictionary.remove(word)) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "NoSuchWord\n";
                }
            } else if (command == "!") {
                std::string action;
                std::string path;
                std::cin >> action >> path;

                if (action == "Save") {
                    if (dictionary.save(path)) {
                        std::cout << "OK\n";
                    } else {
                        std::cout << "ERROR: Failed to save dictionary to "
                                  << path << '\n';
                    }
                } else if (action == "Load") {
                    if (dictionary.load(path)) {
                        std::cout << "OK\n";
                    } else {
                        std::cout << "ERROR: Failed to load dictionary from "
                                  << path << '\n';
                    }
                }
            } else {
                std::string word = command;
                to_lower(word);

                auto [success, value] = dictionary.find(word);
                if (success) {
                    std::cout << "OK: " << value << "\n";
                } else {
                    std::cout << "NoSuchWord\n";
                }
            }
        } catch (const std::bad_alloc& e) {
            std::cout << "ERROR: Out of memory (" << e.what() << ")\n";
        } catch (const std::exception& e) {
            std::cout << "ERROR: System error occurred (" << e.what() << ")\n";
        }
    }

    return 0;
}
