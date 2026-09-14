//============================================================================
// Algoritmo: Aho-Corasick + SQRT Decomposition (Offline Queries)
// Tempo:   O(S + Q log Q + N + K + Q sqrt(N)) | N = |T|, S = soma dos genes
// Memória: O(S + N + Q)
// Resultado : Accepted | 296 ms | 16100 KB
//============================================================================

#include <bits/stdc++.h>
using namespace std;

const int MAXT = 1e5 + 1;
const int MAXS = 5 * MAXT;
const int INF = 1e6 + 9;
const int B = 318; // Tamanho do bloco: ceil(sqrt(10^5))

// --- Estruturas do Aho-Corasick ---
int trie[MAXS][4];
int failure[MAXS];
int output[MAXS];     // Menor ID do gene que termina no nó
int outPrefix[MAXS];  // Link de dicionário (atalho para ancestral com gene)
int geneLengths[MAXS];
int nodeCount = 0;

// --- Estrutura da SQRT Decomposition ---
int val[MAXT];
int blockMin[B + 1];
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

// Atualização O(1): válida porque os valores em val[i] só diminuem (monotônicos)
void update(int i, int k) {
    if (val[i] <= k) return;
    val[i] = k;
    blockMin[i / B] = min(blockMin[i / B], k);
}

// Consulta em O(sqrt(N))
int query(int l, int r) {
    int res = INF;
    int lb = l / B;
    int rb = r / B;

    // Caso 1: Intervalo contido em um único bloco
    if (lb == rb) {
        for (int i = l; i <= r; i++) res = min(res, val[i]);
        return res;
    }

    // Caso 2: Intervalo abrange múltiplos blocos
    for (int i = l; i < (lb + 1) * B; i++) res = min(res, val[i]); // Rebarba esquerda
    for (int i = lb + 1; i < rb; i++) res = min(res, blockMin[i]); // Blocos completos intermediários
    for (int i = rb * B; i <= r; i++) res = min(res, val[i]);      // Rebarba direita
    
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

    // Inicialização da SQRT Decomposition
    for (int i = 0; i < MAXT; i++) val[i] = INF;
    for (int i = 0; i < B + 1; i++) blockMin[i] = INF;

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

    // Ajusta limites para indexação 0-based
    for (int i = 0; i < Q; i++) {
        int l, r;
        cin >> l >> r;
        queries[i] = {l - 1, r - 1, i};
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

        // Atualiza a SQRT com o gene que termina na posição i (0-indexed)
        int out = output[curState];
        if (out) {
            int startPos = i - geneLengths[out] + 1;
            update(startPos, out);
        }

        // Percorre a cadeia de sufixos correspondentes a outros genes
        int auxState = curState;
        while (outPrefix[auxState]) {
            int prefixState = outPrefix[auxState];
            int geneId = output[prefixState];
            int startPos = i - geneLengths[geneId] + 1;
            update(startPos, geneId);
            auxState = prefixState;
        }

        // Responde a todas as queries que terminam exatamente na posição i
        while (curQuery < queries.size() && queries[curQuery].r == i) {
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