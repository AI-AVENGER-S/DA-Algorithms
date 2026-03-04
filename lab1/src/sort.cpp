#include "../include/sort.hpp"

unsigned char getByte(unsigned long long key, int byteNum) {
    return (key >> (byteNum * 8)) & 0xFF;
}

void radixSort(Vector<Item>& a) {
    size_t n = a.size();
    if (n <= 1) return;

    Vector<Item> b(n);

    for (int byteNum = 0; byteNum < 8; ++byteNum) {
        size_t count[256] = {0};

        for (size_t i = 0; i < n; ++i) {
            count[getByte(a[i].key, byteNum)]++;
        }

        for (int i = 1; i < 256; ++i) {
            count[i] += count[i - 1];
        }

        for (size_t i = n; i > 0; --i) {
            unsigned char byte = getByte(a[i - 1].key, byteNum);
            b[--count[byte]] = std::move(a[i - 1]);
        }

        for (size_t i = 0; i < n; ++i) {
            a[i] = std::move(b[i]);
        }
    }
}