#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

void solve() {

    ll n;
    cin >> n;

    vector<vector<ll>> a(n+1);
    vector<pair<ll, ll>> edges;
    vector<vector<ll>> keys(n+1);

    for (ll i = 0; i < n-1; i++) {
        ll x, y;
        cin >> x >> y;
        a[x].push_back(y);
        a[y].push_back(x);
        edges.push_back({x, y});
    }


    // Fazer um bfs n-1 vezes, para cada aresta, assim conseguimos montar as chaves de forma correta para cada nodo]
    ll k = 1;
    for (auto edge : edges) {

        queue<ll> pq;
        pq.push(edge.first);
        vector<bool> v(n+1, false);
        v[edge.first] = true;

        while (!pq.empty()) {
        

            ll front = pq.front();
            pq.pop();

            for (auto u : a[front]) {
                if (u == edge.second || v[u]) continue;
                v[u] = true;
                pq.push(u);
            }
        }

        // com isso, no vetor de visitados temos quem faz parte desse componente conexo
        for (int i = 1; i <= n; i++) {
            if (v[i]) {
                keys[i].push_back(k);     // Chave pro componente 1
            } else {
                keys[i].push_back(k + 1); // Chave pro componente 2
            }
        }
        k += 2;
    }

    cout << k-1 << endl;

    for (int i = 1; i <= n; i++) {
        cout << keys[i].size() << ' ';
        for (auto j : keys[i]) cout << j << ' ';
        cout << '\n';
    }

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}