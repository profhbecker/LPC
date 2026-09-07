//|----------------------------------------------------|
//| Time: O(M * N)          Memory: O(M + N)           |
//| Resultado: Time Limit Exceeded on test 29          |
//|----------------------------------------------------|

#include <bits/stdc++.h>
 
using namespace std;
typedef long long ll;
typedef pair<ll, int> li;
 
typedef struct str_ch{
    int l, r, k;
    ll x;
    str_ch(int l, int r, ll x, int k): l(l), r(r), x(x), k(k) {} 
} CHANNEL;
 
const ll INF = 1e19+9;
const int MAXN = 1e5+1;
vector<CHANNEL> channels[MAXN];
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m, a, b;
    cin >> n >> m >> a >> b;
 
    for(int i=0; i<m; i++) {
        int u, l, r, k;
        ll x;
        cin >> u >> l >> r >> x >> k;
        channels[u].emplace_back(l, r, x, k);
    }
 
    vector<ll> dist(n+1, INF);
    dist[1] = 0;
    ll minA=0, minB=0;
    bool foundA = false, foundB = false;
    priority_queue<li, vector<li>, greater<li>> q;
    q.emplace(0, 1); // custo atual, nó atual
    while(!q.empty()) {
        li cur = q.top();
        q.pop();
        if(cur.first > dist[cur.second]) continue;
        if(cur.second == a) {
            minA = cur.first;
            foundA = true;
        }
        if(cur.second == b) {
            minB = cur.first;
            foundB = true;
        }
        if(foundA && foundB) {
            cout << minA + minB << "\n";
            return 0;
        }
 
        for(auto ch : channels[cur.second]) {
            for(int j=0; j<=ch.r - ch.l; j++) {
                ll cost = cur.first + ch.x + j*ch.k;
                if(cost < dist[ch.l+j]) {
                    dist[ch.l+j] = cost;
                    q.emplace(cost, ch.l+j);
                }
            }
        }
    }
 
    cout << -1 << "\n";
}