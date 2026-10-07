#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n;
    cin >> n;

    long long p = 1;

    while (p * 3 <= n)
        p *= 3;

    if (n <= 2 * p)
        cout << n + p << '\n';
    else
        cout << 3 * n - 3 * p << '\n';

    return 0;
}