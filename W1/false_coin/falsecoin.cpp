/* Detect false coin
 * 
 * The strategy is as follows:
 * False coin has weight greater or less than all the others.
 * It never appears in equality; neither does it change its relation to others (> or <).
 * Thus, we first rule out those coins appearing in equality; for the remaining coins,
 * we have two cases
 * a) false coin appears at least once in weightings: we look for the coin that is heavier or
 * lighter all the time; however, the resulting coins may not be unique. For example, given 6 coins with 5 being false, we may have 1,2 < 3,5 1,6 < 3,5 4,2 = 1,6, 1 < 5. 3 and 5 never appear in equality and they seem consistently heavier. We can however break the tie since 5 is heavier
 * in more weightings than 3. To sum up, we look for the coin that is heavier or lighter in more
 * weightings than others.
 * b) false coin never appears in any weightings: with the same philosophy as in a), we choose
 * the coin to be false which never appears while no other appearing coin is susceptible to be
 * false.
 * Following the above argument, we can also infer the unidentifiable case, where multiple coins 
 * meet the conditions a and b. 
 * g++ -Wall -O0 -g -o falsecoin falsecoin.cpp
 */

#include <iostream>
#include <vector>
#include <algorithm> // set_difference, sort
using namespace std;

#define forloop(i, l, h) for (int i=(l); i< (h); i++)
#define rep(i, N) forloop(i, 0, N)

vector<int> difference(vector<int>& first, vector<int>& second) {
    vector<int>::iterator it;
    it = set_difference(first.begin(), first.end(), second.begin(), second.end(), first.begin());
    first.resize(it - first.begin());
    return first;
}

/* Wrong implementation
 * Did not consider the case where several coins appear only once or fewer times
 * than the real false coin =.=
 */
void falsecoin0() {
    int n, k; // # of coins and weightings; coin identifier from 1 to n
    int P;
    char result; // < > or =
    cin >> n >> k;
    vector<int> greater(n), less(n);
    rep(i, n) {
        greater[i] = i+1;
        less[i] = i+1;
    }
    rep(i, k) {
        cin >> P; // # of coins in this weighting
        vector<int> left(P), right(P);
        rep(j, P) {
            cin >> left[j];
        }
        rep(j, P) {
            cin >> right[j];
        }
        cin >> result;
        sort(left.begin(), left.end()); // must sort!
        sort(right.begin(), right.end());
        if (result == '=') {
            greater = difference(greater, left);
            greater = difference(greater, right);
            less = difference(less, left);
            less = difference(less, right);
        }
        else if (result == '>') {
            greater = difference(greater, right);
            less = difference(less, left);
        }
        else {
            greater = difference(greater, left);
            less = difference(less, right);
        }
    }
    cout<<"greater"<<endl;
    rep(i, greater.size()) cout<<greater[i]<<endl;
    cout<<"less"<<endl;
    rep(i, less.size()) cout<<less[i]<<endl;
    /*
    if (remcoins.size() == 1) cout << remcoins[0];
    else cout << 0;
    */
}

void falsecoin() {
    int n, k; // # of coins and weightings; coin identifier from 1 to n
    int P;
    char op; // < > or =
    cin >> n >> k;
    vector<int> lt(n+1), gt(n+1), eg(n+1);
    rep(i, k) {
        cin >> P;
        vector<int> tempcoins(P*2);
        rep(j, P*2) {
            cin >> tempcoins[j];
        }
        cin >> op;
        if (op == '=') {
            rep(j, P*2) {
                eg[tempcoins[j]] = 1;
            }
        }
        else if (op == '>') {
            rep(j, P) {
                gt[tempcoins[j]]++;
                lt[tempcoins[j+P]]++;
            }
        }
        else {
            rep(j, P) {
                lt[tempcoins[j]]++;
                gt[tempcoins[j+P]]++;
            }
        }
    }
    /*
    cout<<"lt"<<endl;
    rep(i, n) cout<< lt[i] << endl;
    cout<<"gt"<<endl;
    rep(i, n) cout<< gt[i] << endl;
    cout<<"eg"<<endl;
    rep(i, n) cout<< eg[i] << endl;
    */
    // it is possible that the false coin does not appear in any weighting...
    // 1,2 = 3,4 given 5 coins
    int falseid = 0, maxcmp = -1, cmp = 0, nFalse = 0;
    forloop(i, 1, n+1) {
        if (eg[i] == 0) {
            if (lt[i] * gt[i] == 0) {
                cmp = lt[i] > gt[i] ? lt[i] : gt[i];
            }
            else cmp = 1;
            if (maxcmp < cmp) {
                maxcmp = cmp;
                falseid = i;
                nFalse = 1;
            }
            else if (maxcmp == cmp) nFalse++; // if multiple coins are possibly false...
        }
    }
    if (nFalse == 1) cout << falseid;
    else cout<< 0;
    
}
int main() {
    std::ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        falsecoin();
        cout << endl;
    }
}
