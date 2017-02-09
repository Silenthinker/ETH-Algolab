/* Build the sum
 * g++ -Wall -O0 -g -o buildsum buildsum.cpp
 */

#include <iostream>
using namespace std;

#define forloop(i, l, h) for (int i=(l); i< (h); i++)
#define rep(i, N) forloop(i, 0, N)

void buildsum(int n) {
    double sum = 0;
    rep(_, n) {
        double fn; // float point number
        cin >> fn;
        sum += fn;
    }
    cout << sum;
}
int main() {
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        int n;
        cin >> n;
        buildsum(n);
        cout << endl;
    }
}
