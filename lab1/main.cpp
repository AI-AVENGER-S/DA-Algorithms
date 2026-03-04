#include <iostream>
#include "../include/vector.hpp"
#include "../include/sort.hpp"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    Vector<Item> items;
    unsigned long long key;
    std::string val;

    while (std::cin >> key) {
        if (std::cin.get() == '\t') {
            std::getline(std::cin, val);
            items.push_back(Item(key, std::move(val)));
        }
    }

    radixSort(items);

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << items[i].key << "\t" << items[i].value << "\n";
    }

    return 0;
}