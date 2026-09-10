// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <climits>

using namespace std;
typedef long long ll;

void solve() {

    ll n, soma = 0;
    cin >> n;
    vector<ll> a(n), b(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        soma += a[i];
    }

    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }

    ll menor = 1e18, ind = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] - b[i] < 0) {
            cout << "-1\n";
            return;
        }
        if (a[i] - b[i] < menor) {
            menor = a[i] - b[i];
            ind = i;
        }
    }

    cout << soma - menor << endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // std::cin >> t;

    while (t--) {
        solve();
    }
}