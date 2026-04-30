#pragma once
#include <vector>
#include <string>
#include <istream>

namespace utils {

struct Position {
    size_t line;
    size_t word_idx;
};

void to_lower_inplace(std::string& s);

std::vector<int> build_z(const std::vector<std::string>& p);

std::vector<int> build_strong_pi(const std::vector<int>& z);

void kmp_stream_search(
    std::istream& in, 
    const std::vector<std::string>& pattern, 
    const std::vector<int>& sp
);

}