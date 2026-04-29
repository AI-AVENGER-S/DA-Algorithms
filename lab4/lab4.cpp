#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "utils.hpp"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    string pattern_line;
    getline(cin, pattern_line);
    
    vector<string> pattern;
    stringstream ss_p(pattern_line);
    string tmp;
    while (ss_p >> tmp) {
        utils::to_lower_inplace(tmp);
        pattern.push_back(tmp);
    }

    if (pattern.empty()) return 0;

    vector<string> text_words;
    vector<utils::Position> text_pos;

    string line;
    uint32_t current_line_num = 1;

    while (getline(cin, line)) {
        stringstream ss_t(line);
        string word;
        uint32_t current_word_idx = 1;

        while (ss_t >> word) {
            utils::to_lower_inplace(word);
            text_words.push_back(word);
            text_pos.push_back({current_line_num, current_word_idx});
            current_word_idx++;
        }
        current_line_num++;
    }

    utils::kmp_search_itmo(pattern, text_words, text_pos);

    return 0;
}