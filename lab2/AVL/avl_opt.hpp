#pragma once


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
        Node(Key&& k, Value&& v) : key(std::move(k)), value(std::move(v)) {}
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

    bool insert(Key key, Value value) {
        if (!root_) {
            root_ = new Node(std::move(key), std::move(value));
            ++size_;
            return true;
        }

        Node* path[128];
        int path_len = 0;

        Node* curr = root_;

        while (curr) {
            path[path_len++] = curr;

            if (key < curr->key) {
                if (!curr->left) {
                    curr->left = new Node(std::move(key), std::move(value));
                    path[path_len++] = curr->left;
                    ++size_;
                    break;
                }
                curr = curr->left;
            } else if (key > curr->key) {
                if (!curr->right) {
                    curr->right = new Node(std::move(key), std::move(value));
                    path[path_len++] = curr->right;
                    ++size_;
                    break;
                }
                curr = curr->right;
            } else {
                return false; 
            }
        }

        for (int i = path_len - 2; i >= 0; --i) {
            Node* node = path[i];
            Node* new_subtree = balance(node);

            if (new_subtree != node) {
                if (i == 0) {
                    root_ = new_subtree;
                } else {
                    Node* parent = path[i - 1];
                    if (parent->left == node) {
                        parent->left = new_subtree;
                    } else {
                        parent->right = new_subtree;
                    }
                }
            }
        }
        return true;
    }


    bool remove(Key key) {
        if (!root_) return false;

        Node* path[128];
        int path_len = 0;

        Node* curr = root_;

        bool found = false;
        Node* target = nullptr;
        while (curr) {
            path[path_len++] = curr;

            if (key < curr->key) {
                curr = curr->left;
            } else if (key > curr->key) {
                curr = curr->right;
            } else {
                found = true;
                target = curr;
                break;
            }
        }
        if (!found) return false;

        Node* parent = (path_len >= 2) ? path[path_len - 2] : nullptr;

        if (!target->left || !target->right) {
            Node* child = target->left ? target->left : target->right;

            if (parent == nullptr) {
                root_ = child;
            } else if (parent->left == target) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            delete target;
        } else {
            Node* succ_parent = target;
            Node* succ = target->right;
            while (succ->left) {
                succ_parent = succ;
                succ = succ->left;
            }

            target->key = std::move(succ->key);
            target->value = std::move(succ->value);

            if (succ_parent->left == succ) {
                succ_parent->left = succ->right;
            } else {
                succ_parent->right = succ->right;
            }
            delete succ;
        }

        --size_;

        for (int i = path_len - 2; i >= 0; --i) {
            Node* node = path[i];
            Node* new_subtree = balance(node);

            if (new_subtree != node) {
                if (i == 0) {
                    root_ = new_subtree;
                } else {
                    Node* p = path[i - 1];
                    if (p->left == node) {
                        p->left = new_subtree;
                    } else {
                        p->right = new_subtree;
                    }
                }
            }
        }

        return true;
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
