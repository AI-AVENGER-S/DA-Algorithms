#include <iostream>
#include <vector>
#include <string>
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

    utils::kmp_stream_search(cin, pattern, sp);

    return 0;
}