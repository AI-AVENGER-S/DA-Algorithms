#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

namespace custom {
    std::vector<int> build_z(const std::vector<std::string>& p) {
        int m = (int)p.size();
        std::vector<int> z(m, 0);
        for (int i = 1, l = 0, r = 0; i < m; ++i) {
            if (i <= r) {
                z[i] = std::min(r - i + 1, z[i - l]);
            }
            while (i + z[i] < m && p[z[i]] == p[i + z[i]]) z[i]++;
            if (i + z[i] - 1 > r) {
                 l = i; 
                 r = i + z[i] - 1; 
            }
        }
        return z;
    }

    std::vector<int> build_strong_pi(const std::vector<std::string>& p) {
        std::vector<int> z = build_z(p);
        int m = (int)z.size();
        std::vector<int> sp(m, 0);
        for (int i = m - 1; i >= 0; --i) {
            if (z[i] > 0) {
                int end_idx = i + z[i] - 1;
                sp[end_idx] = std::max(sp[end_idx], z[i]);
            }
        }
        return sp;
    }

    size_t kmp_search_itmo(const std::vector<std::string>& text, const std::vector<std::string>& pattern, const std::vector<int>& sp) {
        size_t count = 0;
        int m = pattern.size();
        int n = text.size();
        int j = 0;
        for (int i = 0; i < n; ++i) {
            while (j > 0 && pattern[j] != text[i]) j = sp[j - 1];
            if (pattern[j] == text[i]) j++;
            if (j == m) {
                count++;
                j = sp[m - 1];
            }
        }
        return count;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::string line, word;
    std::getline(std::cin, line);
    std::vector<std::string> pattern;
    {
        std::string tmp;
        for (char c : line) {
            if (isspace(c)) {
                if (!tmp.empty()) { pattern.push_back(tmp); tmp.clear(); }
            } else tmp += (char)tolower(c);
        }
        if (!tmp.empty()) pattern.push_back(tmp);
    }

    std::vector<std::string> text;
    while (std::cin >> word) {
        for (char &c : word) c = (char)tolower(c);
        text.push_back(word);
    }

    std::cout << "Text size: " << text.size() << " words\n";
    std::cout << "Pattern size: " << pattern.size() << " words\n";
    std::cout << "-----------------------------------\n";

// ----

    auto sp = custom::build_strong_pi(pattern);
    auto start = std::chrono::high_resolution_clock::now();
    size_t kmp_res = custom::kmp_search_itmo(text, pattern, sp);
    auto end = std::chrono::high_resolution_clock::now();
    auto kmp_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Custom KMP: " << kmp_time << " us (Found: " << kmp_res << ")\n";

// ----

    start = std::chrono::high_resolution_clock::now();
    size_t stl_res = 0;
    auto it = text.begin();
    while ((it = std::search(it, text.end(), pattern.begin(), pattern.end())) != text.end()) {
        stl_res++;
        std::advance(it, 1);
    }
    end = std::chrono::high_resolution_clock::now();
    auto stl_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "STL std::search: " << stl_time << " us (Found: " << stl_res << ")\n";
    std::cout << "-----------------------------------\n";

    if (stl_time > 0) {
        std::cout << "Speedup: " << (double)stl_time / kmp_time << "x\n";
    }

    return 0;
}