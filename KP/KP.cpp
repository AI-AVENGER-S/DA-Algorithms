#include <iostream> 
#include <string> 
#include <vector> 
#include <array>
#include <map>  
#include <unordered_map>
#include <set>           
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <numeric>    
#include <memory>     
#include <queue>       
#include <iomanip> 
#include <deque>
#include <climits>

using namespace std;

#define all(x) x.begin(), x.end()

using ll = long long;
using ld = long double;

using State = tuple<int, int, int>;

const int INF = INT_MAX;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<string> a(n);
    for (auto& el : a) cin >> el;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};


    int q;
    cin >> q;

    vector<int> dist(n * m, INF);
    vector<int> used(n * m, 0);

    int timer = 0;

    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        --x1;
        --y1;
        --x2;
        --y2;

        int start = x1 * m + y1;
        int finish = x2 * m + y2;

        ++timer;

        int h0 = abs(x1 - x2) + abs(y1 - y2);

        deque<pair<int, int>> dq;

        dist[start] = 0;
        used[start] = timer;

        dq.push_front({0, start});

        int ans = -1;

        while (!dq.empty()) {
            auto [d, v] = dq.front();
            dq.pop_front();

            if (used[v] != timer || d != dist[v]) {
                continue;
            }

            if (v == finish) {
                ans = h0 + 2 * d;
                break;
            }

            int x = v / m;
            int y = v % m;

            int h = abs(x - x2) + abs(y - y2);

            for (int k = 0; k < 4; k++) {
                int nx = x + dx[k];
                int ny = y + dy[k];

                if (nx < 0 || nx >= n || ny < 0 || ny >= m) {
                    continue;
                }

                if (a[nx][ny] == '#') {
                    continue;
                }

                int to = nx * m + ny;

                int nh = abs(nx - x2) + abs(ny - y2);

                int w = (nh > h);

                int nd = d + w;

                if (used[to] != timer || nd < dist[to]) {
                    dist[to] = nd;
                    used[to] = timer;

                    if (w == 0) {
                        dq.push_front({nd, to});
                    } else {
                        dq.push_back({nd, to});
                    }
                }
            }
        }

        cout << ans << '\n';
    }

    
    return 0;
}
