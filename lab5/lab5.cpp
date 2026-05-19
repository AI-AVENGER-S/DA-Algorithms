#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

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

    void extend(int i) {
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

        for (int i = 0; i < text.length(); ++i) {
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    string s;
    cin >> s;
    
    cout << solve(s) << "\n";
    
    return 0;
}