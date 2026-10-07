#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

int main()
{
    
    ll n, cn = 0, vn = 0;
    cin >> n;
    vector<double> i(n+1, 0), cc(n+1), vv(n+1);
    for (int j = 1; j <= n; j++) {
        double c, v;
        
        cin >> c >> v;
        
        cc[j] = cc[j-1] + c;
        vv[j] = vv[j-1] + v;
        
        i[j] = (cc[j] - vv[j]) / (cc[j] + vv[j]);
        
        
    }
    
    
    ll q;
    cin >> q;
    
    for (int j = 0; j < q; j++) {
        ll a;
        cin >> a;
        if (i[a] == 0) {
            cout << "NEUTRO\n";
        } else if (i[a] < 0) {
            cout << "VENDA\n";
        } else {
            cout << "COMPRA\n";
        }
    }

    return 0;
}