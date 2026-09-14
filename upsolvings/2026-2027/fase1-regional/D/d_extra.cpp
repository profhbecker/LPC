#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

// 1. Função para rotacionar a matriz 90 graus no sentido horário
vector<vector<char>> rotate90(const vector<vector<char>>& mat) {
    int n = mat.size();
    int m = mat[0].size();
    vector<vector<char>> rot(m, vector<char>(n)); // Dimensões invertidas
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            rot[j][n - 1 - i] = mat[i][j];
        }
    }
    return rot;
}


vector<vector<char>> flip(vector<vector<char>> mat) {
    reverse(mat.begin(), mat.end());
    return mat;
}


void solve() {
    ll n, m;
    cin >> n >> m;

    vector<vector<char>> mat(n, vector<char>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> mat[i][j];
        }
    }

    vector<vector<vector<char>>> rotations;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            rotations.push_back(mat);
            mat = rotate90(mat);
        }
        mat = flip(mat); 
    }

    vector<bool> s(8, true);
    if (n != m) {
        s[1] = s[3] = s[5] = s[7] = false;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 1; k < 8; k++) {
                if (s[k] && rotations[0][i][j] != rotations[k][i][j]) {
                    s[k] = false;
                }
            }
        }
    }

    cout << count(s.begin(), s.end(), true) << "\n";

    

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