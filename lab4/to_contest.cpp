#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

namespace utils {

struct Position {
    size_t line;
    size_t word_idx;
};

void to_lower_inplace(std::string& s) {
    for (char& c : s) {
        c = (char)std::tolower((unsigned char)c);
    }
}

std::vector<int> build_z(const std::vector<std::string>& p) {
    int m = (int)p.size();
    std::vector<int> z(m, 0);
    for (int i = 1, l = 0, r = 0; i < m; ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
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

void kmp_stream_search(std::istream& in, const std::vector<std::string>& pattern, const std::vector<int>& sp) {
    if (pattern.empty()) return;

    const int m = (int)pattern.size();
    std::vector<Position> buffer(m);
    
    std::string line;
    size_t line_num = 1;
    size_t total_words = 0;
    int j = 0;
    
    std::string current_word;
    current_word.reserve(100); 

    while (std::getline(in, line)) {
        size_t word_idx = 1;
        
        for (size_t i = 0; i <= line.length(); ++i) {
            if (i == line.length() || std::isspace((unsigned char)line[i])) {
                if (!current_word.empty()) {
                    to_lower_inplace(current_word);

                    buffer[total_words % m] = {line_num, word_idx};
                    ++total_words;

                    while (j > 0 && pattern[j] != current_word) {
                        j = sp[j - 1];
                    }
                    if (pattern[j] == current_word) {
                        ++j;
                    }

                    if (j == m) {
                        const auto& start_pos = buffer[(total_words - m) % m];
                        std::cout << start_pos.line << ", " << start_pos.word_idx << "\n";
                        j = sp[m - 1];
                    }

                    current_word.clear();
                    word_idx++;
                }
            } else {
                current_word += line[i];
            }
        }
        line_num++;
    }
}
}


using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string pattern_line;
    if (!getline(cin, pattern_line)) {
        return 0;
    }

    vector<string> pattern;
    string tmp;
    for (char c : pattern_line) {
        if (isspace((unsigned char)c)) {
            if (!tmp.empty()) {
                utils::to_lower_inplace(tmp);
                pattern.push_back(tmp);
                tmp.clear();
            }
        } else {
            tmp += c;
        }
    }
    if (!tmp.empty()) {
        utils::to_lower_inplace(tmp);
        pattern.push_back(tmp);
    }

    if (pattern.empty()) {
        return 0;
    }

    vector<int> z = utils::build_z(pattern);
    vector<int> sp = utils::build_strong_pi(z);

    utils::kmp_stream_search(cin, pattern, sp);

    return 0;
}