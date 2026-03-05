#pragma once

#include "vector.hpp"
#include <string>

struct Item {
    unsigned long long key;
    std::string value;

    Item() : key(0) {}
    Item(unsigned long long k, std::string v) : key(k), value(std::move(v)) {}
};

void RadixSort(Vector<Item>& a);