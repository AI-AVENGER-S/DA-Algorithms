#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <utility>

using namespace std;


template <typename T>
class Vector {
private:
    T* data;
    size_t sz;
    size_t cap;

    void reserve(size_t new_cap) {
        if (new_cap <= cap) return;
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < sz; ++i) {
            new_data[i] = move(data[i]);
        }
        delete[] data;
        data = new_data;
        cap = new_cap;
    }

public:
    Vector() : data(nullptr), sz(0), cap(0) {}
    
    explicit Vector(size_t n) : data(new T[n]), sz(n), cap(n) {}

    ~Vector() {
        delete[] data;
    }

    // Конструктор копирования
    Vector(const Vector& other) : data(nullptr), sz(0), cap(0) {
        reserve(other.cap);
        for (size_t i = 0; i < other.sz; ++i) {
            data[i] = other.data[i];
        }
        sz = other.sz;
    }

    // Оператор присваивания (Copy-and-Swap idiom)
    Vector& operator=(Vector other) {
        swap(data, other.data);
        swap(sz, other.sz);
        swap(cap, other.cap);
        return *this;
    }

    void push_back(T value) {
        if (sz == cap) {
            reserve(cap == 0 ? 8 : cap * 2);
        }
        data[sz++] = move(value);
    }

    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    size_t size() const { return sz; }
};

#endif