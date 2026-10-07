#include <bits/stdc++.h>

using namespace std;
typedef long long ll;


bool dfs_kuhn(int u, const vector<vector<int>>& adj, vector<int>& match_r, vector<bool>& vis) {
    for (int v : adj[u]) {
        if (vis[v]) continue;
        vis[v] = true;
        
        // Se v está livre ou se o vértice emparelhado com ele puder achar outro par
        if (match_r[v] < 0 || dfs_kuhn(match_r[v], adj, match_r, vis)) {
            match_r[v] = u;
            return true;
        }
    }
    return false;
}

void solve() {

    ll n, m1, m2;
    cin >> n >> m1 >> m2;

    set<pair<ll, ll>> set_a, set_b;

    for (int i = 0; i < m1; i++) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        set_a.insert({u, v});
    }

    for (int i = 0; i < m2; i++) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        set_b.insert({u, v});
    }

    if (m1 != m2) {
        cout << "-1\n";
        return;
    }

    //Remover as q ja estao no lugar
    vector<pair<int, int>> rem_a, rem_b;
    for (auto edge : set_a) {
        if (set_b.count(edge)) {
            set_b.erase(edge);
        } else {
            rem_a.push_back(edge);
        }
    }
    for (auto edge : set_b) {
        rem_b.push_back(edge);
    }

    int k = rem_a.size();
    if (k == 0) {
        cout << 0 << "\n";
        return;
    }

    //Constroi o grafo bipartido para verificar o fluxo maximo 
    // cada lado eh os vertices de cada set, uma aresta existe se tem algum vertice em comum
    vector<vector<int>> adj(k);
    for (int i = 0; i < k; i++) {
        int u1 = rem_a[i].first, v1 = rem_a[i].second;
        for (int j = 0; j < k; j++) {
            int u2 = rem_b[j].first, v2 = rem_b[j].second;
            if (u1 == u2 || u1 == v2 || v1 == u2 || v1 == v2) {
                adj[i].push_back(j);
            }
        }
    }

    // Executar o Algoritmo de Kuhn
    vector<int> match_r(k, -1);
    int max_matching = 0;
    for (int i = 0; i < k; i++) {
        vector<bool> vis(k, false);
        if (dfs_kuhn(i, adj, match_r, vis)) {
            max_matching++;
        }
    }


    cout << max_matching + (k - max_matching) * 2 << "\n";

    
    return;

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