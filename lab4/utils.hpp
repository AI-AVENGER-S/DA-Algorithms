#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace utils {

struct Position {
    uint32_t line;
    uint32_t word_idx;
};

void to_lower_inplace(std::string& s);

void kmp_search_itmo(
    const std::vector<std::string>& p, 
    const std::vector<std::string>& t, 
    const std::vector<Position>& pos
);

}