/* Play with dominoes
 * 
 * Linearly scan currentPos + h -1 and update maxPos
 * g++ -Wall -O0 -g -o dominoes dominoes.cpp
 */

#include <iostream>
#include <vector>
using namespace std;

#define forloop(i, l, h) for (int i=(l); i< (h); i++)
#define rep(i, N) forloop(i, 0, N)

void dominoes(int n) {
    int maxPos = 1, currentPos = 1;
    int h; // height
    rep(_, n) {
        cin >> h;
        if (maxPos >= currentPos) {
            maxPos = max(maxPos, currentPos + h - 1); // ATTENTION: cannot output directly maxPos since it may be out of bound, ex. 2 3 4 should yield 3 while maxPos is 6
            currentPos++;
        }
    }
    cout << currentPos-1;
    
    
}
int main() {
    std::ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        int n;
        cin >> n;
        dominoes(n);
        cout << endl;
    }
}
