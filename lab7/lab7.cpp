#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

using ll = long long;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    string n;
    int m;
    cin >> n >> m;

    int L = n.size();

    vector<vector<ll>> dp(2, vector<ll>(m, 0));

    dp[0][0] = 1;

    ll ans = 0;

    for (int i = 0; i < L; ++i) {
        vector<vector<ll>> ndp(2, vector<ll>(m, 0));

        int current = n[i] - '0';
        int first = (i == 0 ? 1 : 0);

        for (int smaller = 0; smaller < 2; ++smaller) {
            int last = (smaller ? 9 : current);

            for (int rem = 0; rem < m; ++rem) {
                ll ways = dp[smaller][rem];

                if (ways == 0) continue;

                for (int digit = first; digit <= last; ++digit) {
                    int next_smaller = smaller || (digit < current);
                    int next_rem = (rem * 10 + digit) % m;

                    ndp[next_smaller][next_rem] += ways;
                }
            }
        }

        dp = ndp;

        ans += dp[1][0];

        if (i + 1 < L) {
            ans += dp[0][0];
        }
    }

    cout << ans << '\n';

    return 0;
}
