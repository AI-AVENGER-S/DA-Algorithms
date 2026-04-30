#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "utils.hpp"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string pattern_line;
    getline(cin, pattern_line);

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

    vector<int> z = utils::build_z(pattern);
    vector<int> sp = utils::build_strong_pi(z);
    const int m = (int)pattern.size();

    vector<utils::Position> buffer(m);
    
    string line;
    size_t line_num = 1;
    size_t total_words = 0;
    int j = 0;
    
    string current_word;
    current_word.reserve(100); 

    while (getline(cin, line)) {
        size_t word_idx = 1;
        
        for (size_t i = 0; i <= line.length(); ++i) {
            if (i == line.length() || isspace((unsigned char)line[i])) {
                if (!current_word.empty()) {
                    utils::to_lower_inplace(current_word);

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

                        cout << start_pos.line << ", " << start_pos.word_idx << "\n";
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

    return 0;
}