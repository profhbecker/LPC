#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef tuple<ll, int, int> lii;
typedef tuple<int, int, int> iii;

const int MAXN = 1e5+1;
const ll INF = 1e18+9;
vector<iii> neighbors[MAXN]; // nó, f, w

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    for(int i=0; i<m; i++) {
        int u, v, f, w;
        cin >> u >> v >> f >> w;
        if(u==v) continue; // boa prática para evitar edge case chatos
        neighbors[u].emplace_back(v, f, w);
        neighbors[v].emplace_back(u, f, w);
    }

    vector<vector<ll>> dist(n+1, vector<ll>(k+1, INF));
    priority_queue<lii, vector<lii>, greater<lii>> q; 
    q.emplace(0, 0, 1);    // custo, k atual, nó atual
    while(!q.empty()) {
        lii cur = q.top();
        q.pop();
        ll c_cost = get<0>(cur);
        int c_k = get<1>(cur), c_node = get<2>(cur);

        if(c_node == n) {
            cout << c_cost << "\n";
            return 0;
        }

        for(auto neighbor : neighbors[c_node]) {
            int n_node = get<0>(neighbor), n_f = get<1>(neighbor), n_w = get<2>(neighbor); 
            
            ll cost_f = c_cost + n_f;
            if(cost_f < dist[n_node][c_k]) {
                q.emplace(cost_f, c_k, n_node);
                dist[n_node][c_k] = cost_f;
            }

            ll cost_w = c_cost + n_w;
            if(n_w > 0 && c_k < k &&  cost_w < cost_f && cost_w < dist[n_node][c_k+1]) {
                q.emplace(cost_w, c_k+1, n_node);
                dist[n_node][c_k+1] = cost_w; 
            }
        }
    }
}
