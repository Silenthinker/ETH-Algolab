#include <iostream>
#include <vector>

#define forloop(i, l, h) for(int i = (l); i < (h); i++)
#define rep(i, N) forloop(i, 0, N)

using namespace std;

void corbusier() {
    int n, i, k, h; cin >> n >> i >> k; // # of disks, parameters of modulor; height of disk
    vector<int> disk; disk.reserve(n);
    rep(_, n) {
        cin >> h;
        disk.push_back(h%k);
    }
    
    vector<vector<bool> > M(n, vector<bool>(k));
    rep(j, n) {
        rep(s, k) {
            if(j == 0) M[j][s] = (disk[j] % k) == s;
            else {
                bool temp;
                if(disk[j] > s) temp = M[j-1][k + s - disk[j]];
                else temp = M[j-1][s - disk[j]];
                M[j][s] = M[j-1][s] || temp;
            }
        }
    }
    if (M[n-1][i]) cout << "yes";
    else cout << "no";
    cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t; // # of test cases
    rep(_, t) corbusier();
    return 0;
}