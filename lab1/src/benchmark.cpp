#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include "../include/sort.hpp"
#include "../include/vector.hpp"

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    Vector<Item> input;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        size_t tab_pos = line.find('\t');
        if (tab_pos != std::string::npos) {
            std::string key_str = line.substr(0, tab_pos);
            std::string value = line.substr(tab_pos + 1);
            
            try {
                unsigned long long key = std::stoull(key_str);
                input.Push_Back(Item(key, std::move(value)));
            } catch (...) {
                continue;
            }
        }
    }

    if (input.size() == 0) {
        std::cout << "No valid data provided to benchmark!" << std::endl;
        return 0;
    }

    Vector<Item> input_stl = input;

    std::cout << "Count of lines: " << input.size() << "\n";
    std::cout << "-----------------------------------\n";

    auto start_ts = std::chrono::high_resolution_clock::now();
    RadixSort(input);
    auto end_ts = std::chrono::high_resolution_clock::now();
    
    uint64_t radix_sort_ts = std::chrono::duration_cast<duration_t>(end_ts - start_ts).count();
    std::cout << "Radix Sort (LSD):  " << radix_sort_ts << " " << DURATION_PREFIX << "\n";

    start_ts = std::chrono::high_resolution_clock::now();
    std::stable_sort(input_stl.begin(), input_stl.end(), [](const Item& a, const Item& b) {
        return a.key < b.key;
    });
    end_ts = std::chrono::high_resolution_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>(end_ts - start_ts).count();
    std::cout << "STL Stable Sort:   " << stl_sort_ts << " " << DURATION_PREFIX << "\n";
    
    std::cout << "-----------------------------------\n";
    
    if (radix_sort_ts > 0) {
        double speedup = (double)stl_sort_ts / radix_sort_ts;
        std::cout << "Speedup: Radix is " << speedup << "x faster!\n";
    }

    return 0;
}