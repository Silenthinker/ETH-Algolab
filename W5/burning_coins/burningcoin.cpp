// STL
#include <iostream>
#include <cmath>
#include <climits>
#include <queue> // std::priority_queue
#include <vector>
#include <algorithm>

using namespace std;

// Predefinition
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

void burningcoin() {
    int n; // # of coins
    cin>>n;
    vector<int> cumsum(n,0); // cumulative sum with cumsum[i] = v[0]+...+v[i-1]
    vector<int> v(n); // value of coins
    int sum = 0;
    vector< vector<int> > optval(n,vector<int>(n)); // only the lower half is used
    rep(i,n) {
        cin>>v[i];
        cumsum[i] = sum+v[i];
        sum += v[i];
    }
    
    rep(k,n) { // the k-th diagonal 
        forloop(i,0,n-k) {// j = i+k
            int j = i+k;
            if (k==0) {
                optval[i][j] = v[i]; // main diagonal
            }
            else {
                optval[i][j] = max(v[i]+cumsum[j]-cumsum[i+1]+v[i+1]-optval[i+1][j],
                    v[j]+cumsum[j-1]-cumsum[i]+v[i]-optval[i][j-1]);
            }
        }
    }
    
    cout<<optval[0][n-1]<<endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin>>t;
    rep(_,t) {
        burningcoin();
    }
    return 0;
}