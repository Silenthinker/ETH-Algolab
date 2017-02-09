/* Count even matrices
 * Like how we solve even pairs
 * Given a row i, every block with [0:i, j:k] boils down to solving a even-pairs
 * problem
 * Keep track of number of even sum and odd parial sum S_{j-1} (nEven, nOdd) and counter of even pairs
 * If S_j is even, counter = counter + nEven + 1; nEven++;
 * otherwise, counter = counter + nOdds; nOdds++;
 * g++ -Wall -O0 -g -o evenpairs evenpairs.cpp
 */

#include <iostream>
#include <vector>
using namespace std;

#define forloop(i, l, h) for (int i=(l); i< (h); i++)
#define rep(i, N) forloop(i, 0, N)

void evenmatrices(int n) {
    // (n+1, n+1), only i, j >= 1 is used
    // Given a row, S[j][k] is the sum of columns j to k; likewise, O and E tracks # of even and odd matrices for sum of columns j to k inclusive
    // partsum[i][j] is the sum of all elements with row and col <= i, j
    vector< vector<int> > S(n+1, vector< int >(n+1, 0)), O(n+1, vector< int >(n+1, 0)), E(n+1, vector< int >(n+1, 0)), partsum(n+1, vector< int >(n+1, 0)); 
    int counter = 0, temp;
    rep(i, n) {
        rep(j, n) {
            cin >> temp;
            partsum[i+1][j+1] = partsum[i+1][j] + partsum[i][j+1] - partsum[i][j] + temp;  
        }
    }
    forloop(i, 1, n+1) {
        forloop(j, 1, n+1) {
            forloop(k, j, n+1) {
                S[j][k] = partsum[i][k] - partsum[i][j-1];
                if (S[j][k] % 2 == 0) counter += (++E[j][k]);
                else counter += (O[j][k]++);
            }
        }
    }
    cout << counter;
    
}
int main() {
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        int n;
        cin >> n;
        evenmatrices(n);
        cout << endl;
    }
}
