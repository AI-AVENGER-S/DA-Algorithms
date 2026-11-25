#include <iostream> 
#include <string> 
#include <vector> 
#include <array>
#include <map>  
#include <unordered_map>
#include <set>           
#include <algorithm>
#include <cmath>
#include <numeric>    
#include <memory>     
#include <utility>       
#include <iomanip> 

using namespace std;

#define all(x) x.begin(), x.end()

using ll = long long;
using ld = long double;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<ll> a(n);

    for (auto &x : a) cin >> x;

    sort(all(a));

    ld best = -1;
    array<ll, 3> ans = {0, 0, 0};

    for (int i = 2; i < n; i++) {
        ll x = a[i - 2];
        ll y = a[i - 1];
        ll z = a[i];

        if (x + y <= z) continue;

        ld p = ((ld)x + y + z) * (-(ld)x + y + z) * ((ld)x - y + z)  * ((ld)x + y - z);

        if (p > best) {
            best = p;
            ans = {x, y, z};
        }
    }

    if (best < 0) {
        cout << '0' << '\n';
        return 0;
    }

    ld area = sqrtl(best) / 4.0L;

    cout << fixed << setprecision(3) << area << '\n';
    cout << ans[0] << ' ' << ans[1] << ' ' << ans[2] << '\n';
}
