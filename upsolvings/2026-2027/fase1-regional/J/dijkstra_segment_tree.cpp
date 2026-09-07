//|------------------------------------------------------------|
//| Time: O((N + M*logN)*logN)      Memory: O(N + M*LogN)      |
//| Resultado: Só funciona para K=0                            |
//|------------------------------------------------------------|

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<ll, int> li;

const int MAXN = 1e5+1;
const ll INF = 1e18+9;

int n, m, a, b;
vector<li> adj[MAXN*4+1];

//Mantemos os vertices originais no começo por conveniência e mapeamos a árvore para comecar em n+1;
int get_tree_node(int id) {
    return n+id;
}

// Construção dos nós virtuais representados pela segment tree
void build_tree(int id, int l, int r) {
    int u = get_tree_node(id);

    if(l==r) { // Necessário caso n não seja potencia de 2
        adj[u].emplace_back(0, l);
        return;
    } 
    if(l+1==r) { // chegamos no último nó da árvore
        //mapeia direto para os nós originais
        adj[u].emplace_back(0, l);
        adj[u].emplace_back(0, r);
        return;
    } 

    int mid = (l+r)/2;
    int l_child = get_tree_node(id*2);
    int r_child = get_tree_node(id*2+1);

    adj[u].emplace_back(0, l_child);
    adj[u].emplace_back(0, r_child);

    build_tree(id*2, l, mid);
    build_tree(id*2+1, mid+1, r);
}

void add_interval(int id, int l, int r, int u, int ql, int qr, ll x) {
    if(ql <= l && qr >= r) {
        if(l==r) adj[u].emplace_back(x, l); //nó original
        else adj[u].emplace_back(x, get_tree_node(id)); //nó virtual
        return;
    }
    if(ql > r || qr < l) return;

    int mid = (l+r)/2;
    add_interval(id*2, l, mid, u, ql, qr, x);
    add_interval(id*2+1, mid+1, r, u, ql, qr, x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> a >> b;

    build_tree(1, 1, n);

    for(int i=0; i<m; i++) {
        int u, l, r, k;
        ll x;
        cin >> u >> l >> r >> x >> k;
        add_interval(1, 1, n, u, l, r, x);
    }

        for(int i=0; i<=n*4; i++) {
            cout << i << " -> ";
            for(auto [n_cost, n_node] : adj[i]) {
                cout << n_cost << " " << n_node << "; ";
            }
            cout << endl;
        }

    priority_queue<li, vector<li>, greater<li>> q;
    vector<ll> dist(n*4+5, INF);
    bool foundA=false, foundB=false;
    dist[1] = 0;
    q.emplace(0, 1);

    while(!q.empty()) {
        auto [cost, node] = q.top();
        q.pop();
        if(cost > dist[node]) continue;

        // Condição de terminação
        if(node==a) foundA=true;
        if(node==b) foundB=true;
        if(foundA && foundB) {
            cout << dist[a] + dist[b] << "\n";
            return 0;
        }

        // Checa os vizinhos (relaxation)
        for(auto [n_cost, n_node] : adj[node]) {
            if(cost + n_cost < dist[n_node]) {
                dist[n_node] = cost + n_cost;
                q.emplace(cost + n_cost, n_node);
            }
        }
    }

    // Não é possível chegar em ambos os nós
    cout << -1 << "\n";
    return 0;
}