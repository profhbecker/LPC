// Código gerado por IA!
//|-----------------------------------------------------------------|
//| Time: O(N * log N + M * log² N)         Memory: O(N + M)        |
//| Resultado: Accepted (656 ms / 14100 KB)                         |
//|-----------------------------------------------------------------|

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll INF = 1e18 + 9;
const int MAXN = 1e5 + 5;

// Estrutura que representa a reta de custo: y = m*x + b
struct Line {
    ll m, b;
    ll eval(ll x) const {
        if (b == INF) return INF;
        return m * x + b;
    }
};

// Nó da Li Chao Tree que gerencia a busca pelo menor caminho ativo
struct Node {
    Line line = {0, INF};
    ll tree_min = INF;      // Menor custo ativo na subárvore deste nó
    int left_active = -1;   // ID da folha ativa mais à esquerda no intervalo
    int right_active = -1;  // ID da folha ativa mais à direita no intervalo
};

Node tree[4 * MAXN];

// Atualiza os ponteiros de folhas ativas e recalcula o custo mínimo do nó
void update_node(int id, int l, int r) {
    int lc = 2 * id;
    int rc = 2 * id + 1;
    
    // Propaga as folhas ativas das subárvores filhas para o pai
    if (l < r) {
        tree[id].left_active = (tree[lc].left_active != -1) ? tree[lc].left_active : tree[rc].left_active;
        tree[id].right_active = (tree[rc].right_active != -1) ? tree[rc].right_active : tree[lc].right_active;
    }
    
    // Se não há folhas ativas abaixo deste nó, o custo mínimo é infinito
    if (tree[id].left_active == -1) {
        tree[id].tree_min = INF;
        return;
    }
    
    ll ans = INF;
    if (l < r) {
        ans = min(tree[lc].tree_min, tree[rc].tree_min);
    }
    
    // Como a reta é linear e monótona, seu valor mínimo em um intervalo
    // estará obrigatoriamente em um dos extremos ativos dele
    if (tree[id].line.b != INF) {
        if (tree[id].left_active != -1) {
            ans = min(ans, tree[id].line.eval(tree[id].left_active));
        }
        if (tree[id].right_active != -1) {
            ans = min(ans, tree[id].line.eval(tree[id].right_active));
        }
    }
    
    tree[id].tree_min = ans;
}

// Inicializa a árvore ativando todas as folhas de 1 a N
void build_tree(int id, int l, int r) {
    tree[id].left_active = l;
    tree[id].right_active = r;
    tree[id].tree_min = INF;
    tree[id].line = {0, INF};
    
    if (l == r) return;
    int mid = (l + r) / 2;
    build_tree(2 * id, l, mid);
    build_tree(2 * id + 1, mid + 1, r);
    update_node(id, l, r);
}

// Inserção da reta na subárvore utilizando a propriedade de cruzamento único (Li Chao)
void insert_line(int id, int l, int r, Line new_line) {
    if (tree[id].left_active == -1) return;
    
    int mid = (l + r) / 2;
    if (tree[id].line.b == INF) {
        tree[id].line = new_line;
        update_node(id, l, r);
        return;
    }
    
    bool lef = new_line.eval(l) < tree[id].line.eval(l);
    bool mid_f = new_line.eval(mid) < tree[id].line.eval(mid);
    
    // A reta que vence no ponto médio assume a posição de residente
    if (mid_f) {
        swap(tree[id].line, new_line);
    }
    
    if (l == r) {
        update_node(id, l, r);
        return;
    }
    
    // A reta perdedora é empurrada para o filho onde ela ainda tem chance de vencer
    if (lef != mid_f) {
        insert_line(2 * id, l, mid, new_line);
    } else {
        insert_line(2 * id + 1, mid + 1, r, new_line);
    }
    update_node(id, l, r);
}

// Decompõe a query de intervalo em nós canônicos e delega para insert_line
void add_segment(int id, int l, int r, int ql, int qr, Line line) {
    if (qr < l || r < ql || tree[id].left_active == -1) return;
    if (ql <= l && r <= qr) {
        insert_line(id, l, r, line);
        return;
    }
    int mid = (l + r) / 2;
    add_segment(2 * id, l, mid, ql, qr, line);
    add_segment(2 * id + 1, mid + 1, r, ql, qr, line);
    update_node(id, l, r);
}

// Desativa permanentemente uma folha que teve seu menor caminho finalizado
void deactivate(int id, int l, int r, int v) {
    if (l == r) {
        tree[id].left_active = -1;
        tree[id].right_active = -1;
        tree[id].tree_min = INF;
        return;
    }
    int mid = (l + r) / 2;
    if (v <= mid) deactivate(2 * id, l, mid, v);
    else deactivate(2 * id + 1, mid + 1, r, v);
    update_node(id, l, r);
}

// Encontra qual ID de folha ativa atinge o menor custo global (target)
int find_best_node(int id, int l, int r, ll target) {
    if (l == r) return l;
    
    int mid = (l + r) / 2;
    int lc = 2 * id;
    int rc = 2 * id + 1;
    
    // Se a reta do próprio nó atinge o custo target nos extremos ativos, achamos a folha
    if (tree[id].line.b != INF) {
        if (tree[id].left_active != -1 && tree[id].line.eval(tree[id].left_active) == target) {
            return tree[id].left_active;
        }
        if (tree[id].right_active != -1 && tree[id].line.eval(tree[id].right_active) == target) {
            return tree[id].right_active;
        }
    }
    
    // Caso contrário, busca recursivamente nas subárvores filhas que contém o valor mínimo
    if (tree[lc].left_active != -1 && tree[lc].tree_min == target) {
        return find_best_node(lc, l, mid, target);
    }
    return find_best_node(rc, mid + 1, r, target);
}

// Representação de uma linha de comunicação de saída
struct Edge {
    int l, r;
    ll x, k;
};

int main() {
    // Otimização de Entrada e Saída
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, a, b;
    if (!(cin >> n >> m >> a >> b)) return 0;
    
    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, l, r;
        ll x, k;
        cin >> u >> l >> r >> x >> k;
        adj[u].push_back({l, r, x, k});
    }
    
    // 1. Inicializa a Li Chao Tree
    build_tree(1, 1, n);
    
    // 2. Insere o estado inicial do Dijkstra: Origem (nó 1) com distância 0
    // Isso é representado por uma reta constante y = 0 no ponto [12]
    add_segment(1, 1, n, 1, 1, {0, 0});
    
    vector<ll> dist(n + 1, INF);
    
    // 3. Loop principal do Dijkstra baseado em Li Chao Tree
    while (true) {
        ll best_val = tree[1].tree_min;
        if (best_val == INF) {
            break; // Todos os nós alcançáveis foram processados
        }
        
        // Extrai o ID do nó com o menor caminho mínimo atual
        int u = find_best_node(1, 1, n, best_val);
        dist[u] = best_val;
        
        // Remove o nó u do conjunto de folhas ativas
        deactivate(1, 1, n, u);
        
        // Relaxa as conexões de saída a partir de u
        for (const auto& edge : adj[u]) {
            // Custo para alcançar v em [L, R]: dist[u] + X + (v - L) * K
            // Convertido em formato de reta: K * v + (dist[u] + X - K * L)
            Line l_edge = {edge.k, dist[u] + edge.x - edge.k * edge.l};
            add_segment(1, 1, n, edge.l, edge.r, l_edge);
        }
    }
    
    // 4. Exibe o resultado final para contactar os dois amigos
    if (dist[a] == INF || dist[b] == INF) {
        cout << "-1\n";
    } else {
        cout << dist[a] + dist[b] << "\n";
    }
    
    return 0;
}