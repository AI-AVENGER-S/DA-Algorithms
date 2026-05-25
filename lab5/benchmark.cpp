#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;


struct Node {
    int l, r;
    int slink;
    map<char, int> next;

    Node(int l, int r) : l(l), r(r), slink(-1) {}
};

class SuffixTree {
    string text;
    vector<Node> nodes;
    int leaf_end;
    
    int act_v, act_pos, act_len, rem;

    int edge_length(int v) const {
        if (v == 0) return 0;

        int right = (nodes[v].r == -1) ? leaf_end : nodes[v].r;

        return right - nodes[v].l + 1;
    }

    void extend(size_t i) {
        leaf_end = i;
        ++rem;
        int last_new = -1;

        while (rem > 0) {
            if (act_len == 0) {
                act_pos = i;
            }

            char edge_char = text[act_pos];

            if (!nodes[act_v].next.count(edge_char)) {
                nodes.emplace_back(i, -1);
                nodes[act_v].next[edge_char] = nodes.size() - 1;

                if (last_new != -1) {
                    nodes[last_new].slink = act_v;
                    last_new = -1;
                }
            } else {
                int next_el = nodes[act_v].next[edge_char];
                int elen = edge_length(next_el);

                if (act_len >= elen) {
                    act_pos += elen;
                    act_len -= elen;
                    act_v = next_el;
                    continue; 
                }

                if (text[nodes[next_el].l + act_len] == text[i]) {
                    ++act_len;
                    if (last_new != -1 && act_v != 0) {
                        nodes[last_new].slink = act_v;
                    }
                    break; 
                }

                int split_v = nodes.size();
                nodes.emplace_back(nodes[next_el].l, nodes[next_el].l + act_len - 1);
                nodes[act_v].next[edge_char] = split_v;

                nodes.emplace_back(i, -1);
                nodes[split_v].next[text[i]] = nodes.size() - 1;

                nodes[next_el].l += act_len;
                nodes[split_v].next[text[nodes[next_el].l]] = next_el;

                if (last_new != -1) {
                    nodes[last_new].slink = split_v;
                }
                last_new = split_v;
            }

            --rem;
            
            if (act_v == 0 && act_len > 0) {
                --act_len;
                act_pos = i - rem + 1;
            } else if (act_v != 0) {
                act_v = (nodes[act_v].slink != -1) ? nodes[act_v].slink : 0;
            }
        }
    }

public:
    SuffixTree(const string& s) : text(s) {
        nodes.reserve(text.length() * 2);
        nodes.emplace_back(-1, -1);
        
        act_v = 0;
        act_pos = 0;
        act_len = 0;
        rem = 0;
        leaf_end = -1;

        for (size_t i = 0; i < text.length(); ++i) {
            extend(i);
        }
    }

    string optimal_lecs_split(int s_len) const {
        int curr = 0;
        int len = 0;
        string res;
        res.reserve(s_len);

        while (len < s_len) {
            int next_el = -1;
            
            for (auto const& [c, v_idx] : nodes[curr].next) {
                if (c == '$') continue; 
                next_el = v_idx;
                break;
            }

            if (next_el == -1) break; 

            int elen = edge_length(next_el);
            int take = min(elen, s_len - len);

            res += text.substr(nodes[next_el].l, take);
            len += take;
            curr = next_el;
        }
        return res;
    }
};

string solve(const string& s) {
    if (s.empty()) return "";

    string t = s + s + "$";
    SuffixTree st(t);

    return st.optimal_lecs_split(s.size());
}


string solve_naive(const string& s) {
    if (s.empty()) return "";
    
    vector<string> rotations;
    rotations.reserve(s.length());
    
    for (size_t i = 0; i < s.length(); ++i) {
        rotations.push_back(s.substr(i) + s.substr(0, i));
    }
    
    return *min_element(rotations.begin(), rotations.end());
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    string s;
    if (!getline(cin, s)) {
        cerr << "No input\n";
        return 1;
    }
    
    while (!s.empty() && isspace((unsigned char)s.back())) {
        s.pop_back();
    }
    
    if (s.empty()) {
        cout << "String length: 0\n";
        return 0;
    }
    
    cout << "String length: " << s.length() << "\n";
    cout << "-----------------------------------\n";
    
    auto start1 = chrono::high_resolution_clock::now();
    string result1 = solve(s);
    auto end1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(end1 - start1);
    
    auto start2 = chrono::high_resolution_clock::now();
    string result2 = solve_naive(s);
    auto end2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
    
    cout << "Suffix Tree: " << duration1.count() << " us\n";
    cout << "Naive std::min_element: " << duration2.count() << " us\n";
    cout << "-----------------------------------\n";
    
    double speedup = (double)duration2.count() / duration1.count();
    cout << "Speedup: " << speedup << "x\n";
    
    if (result1 != result2) {
        cerr << "ERROR: Results do not match!\n";
        cerr << "Suffix Tree: " << result1 << "\n";
        cerr << "Naive: " << result2 << "\n";
        return 1;
    }
    
    return 0;
}
