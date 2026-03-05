#include <iostream>
#include <utility>
#include <string>

template <typename T>
class Vector {
private:
    T* data;
    size_t sz;
    size_t cap;

    void Reserve(size_t new_cap) {
        if (new_cap <= cap) return;
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        cap = new_cap;
    }

public:
    Vector() : data(nullptr), sz(0), cap(0) {}
    
    Vector(size_t n) : data(new T[n]), sz(n), cap(n) {}

    ~Vector() {
        delete[] data;
    }

    Vector(const Vector& other) : data(nullptr), sz(0), cap(0) {
        Reserve(other.cap);
        for (size_t i = 0; i < other.sz; ++i) {
            data[i] = other.data[i];
        }
        sz = other.sz;
    }

    Vector& operator=(Vector other) {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
        return *this;
    }

    void Push_Back(T value) {
        if (sz == cap) {
            Reserve(cap == 0 ? 8 : cap * 2);
        }
        data[sz++] = std::move(value);
    }

    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    size_t size() const { return sz; }

    T* begin() { return data; }
    T* end() { return data + sz; }
    const T* begin() const { return data; }
    const T* end() const { return data + sz; }
};


struct Item {
    unsigned long long key;
    std::string value;

    Item() : key(0) {}
    Item(unsigned long long k, std::string v) : key(k), value(std::move(v)) {}
};



unsigned char GetByte(unsigned long long key, int byteNum) {
    return (key >> (byteNum * 8)) & 0xFF;
}

void RadixSort(Vector<Item>& a) {
    size_t n = a.size();
    if (n <= 1) return;

    Vector<Item> b(n);

    for (int byteNum = 0; byteNum < 8; ++byteNum) {
        size_t count[256] = {0};

        for (size_t i = 0; i < n; ++i) {
            count[GetByte(a[i].key, byteNum)]++;
        }

        for (int i = 1; i < 256; ++i) {
            count[i] += count[i - 1];
        }

        for (size_t i = n; i > 0; --i) {
            unsigned char byte = GetByte(a[i - 1].key, byteNum);
            b[--count[byte]] = std::move(a[i - 1]);
        }

        for (size_t i = 0; i < n; ++i) {
            a[i] = std::move(b[i]);
        }
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    Vector<Item> items;
    unsigned long long key;
    std::string val;

    while (std::cin >> key) {
        if (std::cin.get() == '\t') {
            std::getline(std::cin, val);
            items.Push_Back(Item(key, std::move(val)));
        }
    }

    RadixSort(items);

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << items[i].key << "\t" << items[i].value << "\n";
    }

    return 0;
}