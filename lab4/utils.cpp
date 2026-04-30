#include "utils.hpp"
#include <iostream>
#include <algorithm>

namespace utils {

void to_lower_inplace(std::string& s) {
    for (char& c : s) {
        c = (char)std::tolower((unsigned char)c);
    }
}

std::vector<int> build_z(const std::vector<std::string>& p) {
    size_t m = p.size();
    std::vector<int> z(m, 0);
    for (size_t i = 1, l = 0, r = 0; i < m; ++i) {
        if (i <= r) {
            z[i] = std::min((int)(r - i + 1), z[i - l]);
        }
        while (i + z[i] < m && p[z[i]] == p[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

std::vector<int> build_strong_pi(const std::vector<int>& z) {
    size_t m = z.size();
    std::vector<int> sp(m, 0);
    
    for (size_t i = 1; i < m; ++i) {
        if (z[i] > 0) {
            int end_idx = i + z[i] - 1;
            sp[end_idx] = std::max(sp[end_idx], z[i]);
        }
    }
    return sp;
}

void kmp_search_itmo(const std::vector<std::string>& p, const std::vector<std::string>& t, const std::vector<Position>& pos) {
    if (p.empty() || t.empty() || p.size() > t.size()) return;

    std::vector<int> z = build_z(p);
    std::vector<int> sp = build_strong_pi(z);

    size_t n = t.size();
    size_t m = p.size();
    
    size_t start = 0;
    int j = 0;

    while (start <= n - m) {
        while (j < m && p[j] == t[start + j]) {
            j++;
        }
        
        if (j == m) {
            std::cout << pos[start].line << ", " << pos[start].word_idx << "\n";
            int shift = m - sp[m - 1];
            start += shift;
            j = sp[m - 1];

        } 
        else {
            if (j == 0) start++;

            else {
                int shift = j - sp[j - 1];
                start += shift;
                j = sp[j - 1];
            }
        }
    }
}

}