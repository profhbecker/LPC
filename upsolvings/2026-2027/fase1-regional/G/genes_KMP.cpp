#include  <bits/stdc++.h>
#include <algorithm>

using namespace std;

int cToI(char c) {
    if(c=='A') return 0;
    if(c=='C') return 1;
    if(c=='T') return 2;
    return 3;
}

typedef struct str_gene{
    string str;
    vector<int> failure;
    str_gene(string s): str(s) {
        failure.assign(s.size(), 0);
        int prefix=0;
        for(int i=1; i<s.size(); i++) {
            if(s[i]==s[prefix]) failure[i] = ++prefix;
            else while(prefix>0) {
                prefix = failure[prefix-1];
                if(s[i] == s[prefix]) {
                    failure[i] = ++prefix;
                    break;
                }
            }
        }
    }
} GENE;

const int MAX = 1e5+1;
int tree[MAX*4+4];
vector<GENE> genes;
string T;
int G;

int compare(int l, int r, int mid, int cur) {
    l--; r--; mid--;
    for(int i=0; i<G; i++) {
        int len = r-l+1;
        int m = genes[i].str.size();
        if(i+1 == cur) return cur;
        if(m > len) continue;

        int pos = max(mid - m, l), state = 0;
        while(pos<=r && pos<=mid+m) {
            if(T[pos] == genes[i].str[state]) {
                state++;
                pos++;
            } else if(state==0) pos++;
            else state = genes[i].failure[state-1];
            
            if(state == m) return i+1; // match completo
        }
    }
    return -1;
}

void build(int id, int l, int r) {
    int mid = (l+r)/2;
    if(l==r) tree[id] = compare(l, r, mid, -1);
    else {;
        int lc = id*2, rc = id*2+1;
        build(lc, l, mid);
        build(rc, mid+1, r);

        if(tree[lc]==-1) tree[id] = tree[rc];
        else if(tree[rc]==-1) tree[id] = tree[lc];
        else tree[id] = min(tree[lc], tree[rc]);
        tree[id] = compare(l, r, mid, tree[id]);        
    }
}

int query(int id, int l, int r, int ql, int qr) {
    if(qr < l || ql > r) return 0;
    if(ql <= l && qr >= r) return tree[id];
    int mid = (l+r)/2;
    int lc = query(id*2, l, mid, ql, qr);
    int rc = query(id*2+1, mid+1, r, ql, qr);
    int cur;
    if(lc==0) return rc;
    if(rc==0) return lc;
    if(lc==-1) cur = rc;
    else if(rc==-1) cur = lc;
    else cur = min(lc, rc);
    return compare(max(l, ql), min(r, qr), (l+r)/2, cur);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T >> G;
    genes.reserve(G);
    for(int i=0; i<G; i++) {
        string str;
        cin >> str;
        genes.emplace_back(str);
    }

    build(1, 1, T.size());

    int q;
    cin >> q;
    for(int i=0; i<q; i++) {
        int a, b;
        cin >> a >> b;
        cout << query(1, 1, T.size(), a, b) << "\n";
    }
}