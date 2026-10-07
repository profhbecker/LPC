// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <climits>
#include <map>

using namespace std;
typedef long long ll;

void solve() {

    ll n, m;
    cin >> n >> m;

    vector<vector<char>> mat(n, vector<char>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> mat[i][j];
        }
    }
    
    ll out = 0;
    
    // Verificar a simetria horizontal, mostrando se ele pode ser rotacionado em 180
    bool sai = false;
    for (int i = 0; i < n / 2; i++) {
        for (int j = 0; j < m; j++) {
            int ni = n - 1 - i;
            if (mat[i][j] != mat[ni][j]) {
                sai = true;
            }
        }
        if (sai) break;
    }
    if (!sai) out++;

    //Verificar a simetria com a vertical, indicando se ele pode ser rotacionado em 90 e -90
    sai = false;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m / 2; j++) {
            int nj = m - 1 - j;
            if (mat[i][j] != mat[i][nj]) {
                sai = true;
            }
        }
        if (sai) break;
    }

    if (!sai) out++;


    // Se a matriz for quadrada, podemos verificar a simetria com as diagonais
    if (n == m) {
        // Verificar a simetria com a diagonal principal
        sai = false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (mat[i][j] != mat[j][i]) {
                    sai = true;
                }
            }
            if (sai) break;
        }
        if (!sai) out++;

        // Verificar a simetria com a diagonal secundária
        sai = false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                int g = n - 1 - (i + j);
                if (mat[i][j] != mat[i + g][j + g]) {
                    sai = true;
                }
            }
            if (sai) break;
        }
        if (!sai) out++;
    }


    switch(out) {
        case 0:
            cout << "1\n";
            break;
        case 1:
            cout << "2\n";
            break;
        case 2:
            cout << "4\n";
            break;
        case 4:
            cout << "8\n";
            break;
    }



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