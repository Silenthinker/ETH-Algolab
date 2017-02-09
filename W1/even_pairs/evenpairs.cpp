/* Count even pairs
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

void evenpairs(int n) {
    int sum = 0, nEven = 0, nOdd = 0, counter = 0;
    int x; // either 0 or 1
    rep(i, n) {
        cin >> x;
        sum += x;
        if (sum % 2 == 0) counter += (++nEven);
        else counter += (nOdd++);
    }
    cout << counter;
    
}
int main() {
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        int n;
        cin >> n;
        evenpairs(n);
        cout << endl;
    }
}
