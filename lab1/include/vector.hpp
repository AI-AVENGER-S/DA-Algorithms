#pragma once

#include <iostream>
#include <utility>


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
            new_data[i] = std::move(data[i]);
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

    Vector(const Vector& other) : data(nullptr), sz(0), cap(0) {
        reserve(other.cap);
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

    void push_back(T value) {
        if (sz == cap) {
            reserve(cap == 0 ? 8 : cap * 2);
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
