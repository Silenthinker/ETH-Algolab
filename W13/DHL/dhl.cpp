#include <iostream>
#include <vector>
#include <climits>

// Namespaces
using namespace std;

// User defined
#define forloop(i, l, h) for (int i = (l); i < (h); i ++)
#define rep(i, N) forloop(i, 0, N)

void dhl() {
    int n; cin >> n;
    vector<int> A, B; A.reserve(n); B.reserve(n);
    vector<int> S_a(n+1), S_b(n+1);
    rep(i, n) {
        int a; cin >> a;
        A[i] = a - 1;
        S_a[i+1] = S_a[i] + A[i];
    }
    rep(i, n) {
        int b; cin >> b;
        B[i] = b - 1;
        S_b[i+1] = S_b[i] + B[i];
    }
    vector<vector<int> > D(n+1, vector<int>(n+1, 0)); // D[i][j]: optimum when i and j parcels remain, respectively.
    forloop(i, 1, n+1) {
        forloop(j, 1, n+1) {
            int mincost = D[0][0] + S_a[i] * S_b[j];
            forloop(s, 1, i) {
                forloop(t, 1, j) {
                    mincost = min(mincost, D[s][t] + (S_a[i] - S_a[s]) * (S_b[j] - S_b[t]));
                }
            }
            D[i][j] = mincost;
        }
    }
    cout << D[n][n] << "\n";
}


int main() {
    int t; cin >> t;
    rep(_, t) dhl();
    return 0;
}