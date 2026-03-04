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

        size_t currentPos = 0;
        for (int i = 0; i < 256; ++i) {
            size_t countTmp = count[i];
            count[i] = currentPos;
            currentPos += countTmp;
        }

        for (size_t i = 0; i < n; ++i) {
            unsigned char byte = getByte(a[i].key, byteNum);
            b[count[byte]++] = std::move(a[i]);
        }

        for (size_t i = 0; i < n; ++i) {
            a[i] = std::move(b[i]);
        }
    }
}