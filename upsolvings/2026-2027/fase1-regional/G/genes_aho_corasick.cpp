//============================================================================
// Algoritmo: Aho-Corasick + Segment Tree Iterativa (Offline Queries)
// Tempo:   O(S + Q log Q + (N + K) log N) 
// Memória: O(S + N + Q)
// Resultado: Accepted | 2375 ms | 17300 KB
//============================================================================

#include <bits/stdc++.h>
using namespace std;

const int MAXT = 1e5 + 1;
const int MAXS = 5 * MAXT;
const int INF = 1e6 + 9;

// --- Estruturas do Aho-Corasick ---
int trie[MAXS][4];
int failure[MAXS];
int output[MAXS];     // Guarda o menor ID do gene que termina no estado
int outPrefix[MAXS];  // Link de dicionário (atalho para o próximo ancestral com gene)
int geneLengths[MAXS];
int nodeCount = 0;

// --- Estrutura da Segment Tree Iterativa ---
int tree[4 * MAXT];
string T;
int G;

// Mapeia caracteres para índices [0..3]
int cToI(char c) {
    if (c == 'A') return 0;
    if (c == 'C') return 1;
    if (c == 'T') return 2;
    return 3;
}

// Insere padrão na Trie guardando apenas o menor ID de gene por nó
void insert(const string& s, int index) {
    int curState = 0;
    for (char c : s) {
        int idx = cToI(c);
        if (!trie[curState][idx]) {
            trie[curState][idx] = ++nodeCount;
        }
        curState = trie[curState][idx];
    }
    if (output[curState]) output[curState] = min(output[curState], index);
    else output[curState] = index;
}

// Constrói os links de falha e a cadeia de dicionário (outPrefix) via BFS
void build_links() {
    queue<int> q;
    for (int i = 0; i < 4; i++) {
        if (trie[0][i]) q.push(trie[0][i]);
    }

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            if (trie[node][i]) {
                int prox = trie[node][i];
                int fail = failure[node];
                
                while (!trie[fail][i] && fail != 0) fail = failure[fail];
                failure[prox] = trie[fail][i];

                // Propaga o link de dicionário atalhando nós sem output
                if (output[failure[prox]]) {
                    outPrefix[prox] = failure[prox];
                } else {
                    outPrefix[prox] = outPrefix[failure[prox]];
                }
                q.push(prox);
            }
        }
    }
}

// --- Segment Tree Bottom-Up (Point Update, Range Minimum Query) ---
void update(int i, int k) {
    int p = i + T.size() - 1; // Mapeia índice 1-based para a folha da árvore
    if (k >= tree[p]) return; // Otimização: só atualiza se o ID for menor

    tree[p] = k;
    for (p /= 2; p > 0; p /= 2) {
        tree[p] = min(tree[p * 2], tree[p * 2 + 1]);
    }
}

int query(int l, int r) {
    l--; // Transforma limite esquerdo em intervalo semi-aberto [l-1, r)
    int res = INF;
    for (l += T.size(), r += T.size(); l < r; l /= 2, r /= 2) {
        if (l % 2 == 1) res = min(res, tree[l++]);
        if (r % 2 == 1) res = min(res, tree[--r]);
    }
    return res;
}

// Estrutura para armazenar e ordenar consultas por ponto final (r)
struct QUERY {
    int l, r, id;
    bool operator<(const QUERY& other) const {
        return r < other.r;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T >> G;
    for (int i = 0; i <= T.size() * 4; i++) tree[i] = INF;

    for (int i = 1; i <= G; i++) {
        string x;
        cin >> x;
        geneLengths[i] = x.size();
        insert(x, i);
    }

    int Q;
    cin >> Q;
    vector<QUERY> queries(Q);
    vector<int> answers(Q);
    for (int i = 0; i < Q; i++) {
        int l, r;
        cin >> l >> r;
        queries[i] = {l, r, i};
    }

    // Processamento Offline: ordena queries pelo término r
    sort(queries.begin(), queries.end());

    build_links();

    // Varredura do texto T simulando o Autômato
    int curState = 0, curQuery = 0;
    for (int i = 0; i < T.size(); i++) {
        if (curQuery >= queries.size()) break;

        int c = cToI(T[i]);

        while (!trie[curState][c] && curState != 0) curState = failure[curState];
        if (trie[curState][c]) curState = trie[curState][c];

        // Atualiza a SegTree com o gene que termina na posição i
        int out = output[curState];
        if (out) {
            int startPos = i - geneLengths[out] + 2; // Posição inicial (1-indexed)
            update(startPos, out);
        }

        // Percorre a cadeia de sufixos correspondentes a outros genes
        int auxState = curState;
        while (outPrefix[auxState]) {
            int prefixState = outPrefix[auxState];
            int geneId = output[prefixState];
            int startPos = i - geneLengths[geneId] + 2;
            update(startPos, geneId);
            auxState = prefixState;
        }

        // Responde a todas as queries que terminam exatamente na posição i+1
        while (curQuery < queries.size() && queries[curQuery].r == i + 1) {
            auto q = queries[curQuery];
            answers[q.id] = query(q.l, q.r);
            curQuery++;
        }
    }

    // Impressão dos resultados na ordem original das queries
    for (int x : answers) {
        cout << (x == INF ? -1 : x) << "\n";
    }

    return 0;
}