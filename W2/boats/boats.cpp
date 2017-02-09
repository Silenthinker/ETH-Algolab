/* Boats
 * 
 * Greedy algorithm, always choose the boat which has the nearest further endpoint.
 * g++ -Wall -O0 -g -o boats boats.cpp
 */

#include <iostream>
#include <vector>
#include <algorithm> // set_difference, sort
#include <climits>
#include <queue> // std::queue
#include <utility> // std::pair
using namespace std;

#define forloop(i, l, h) for (int i=(l); i< (h); i++)
#define rep(i, N) forloop(i, 0, N)

bool myFunction (const pair<int, int>& lhs, const pair<int, int>&rhs) {
    return (lhs.second < rhs.second);
}

void boats() {
    int n; // # of wizards
    cin >> n;
    int l, p; // length of boat and ring's position
    int startpos, maxNumBoats = 1; // at least one boat can be tied
    vector<pair<int, int> > lenAndPos(n);
    rep(i, n) {
        cin >> l >> p;
        lenAndPos[i] = make_pair(l, p);
    }
    sort(lenAndPos.begin(), lenAndPos.end(), myFunction);
    startpos = lenAndPos[0].second; 
    int i = 1;
    while (i < n) {
        l = lenAndPos[i].first; p = lenAndPos[i].second;
        // if start endpoint surpasses ring position, continue
        if (startpos <= p) {
            if (l + startpos <= p || i == n-1) { // have enough space or it is last boat
                startpos = p;
            }
            else {
                int minStartpos = max(l + startpos, p);
                forloop(j, i+1, n) {
                    int nextL = lenAndPos[j].first, nextP = lenAndPos[j].second;
                    int temp = max(nextL + startpos, nextP);
                    if (minStartpos > temp) {
                        minStartpos = temp;
                        i = j;
                    }
                }
                startpos = minStartpos;
            }
            maxNumBoats++;
        }
        i++;
        
        //cout << "startpos: " << startpos << endl;
    }
    cout << maxNumBoats;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        boats();
        cout << endl;
    }
}
