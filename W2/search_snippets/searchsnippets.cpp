/* Search minimum window containing all query terms
 * 
 * Sliding window, move the index of minimum position forward 
 * use priority_queue to efficiently find minimum position
 * g++ -Wall -O0 -g -o searchsnippets searchsnippets.cpp
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

/* Compare pair<term_id, pos>
 * smaller pos should be at top
 */
struct mycomparison {
  bool operator() (const pair<int, int>& lhs, const pair<int, int>&rhs) const
  {
    return (lhs.second > rhs.second);
  }
};

void searchsnippets() {
    int n; // # of words in query
    cin >> n;
    vector<int> tf(n), idx(n);
    int minTermIdx, minPos = INT_MAX, maxPos = INT_MIN, len; // minTermIdx: index of term whose position is minimum; idx[minTermIdx]: index of position vector; posting[minTermIdx][idx[minTermIdx]]: minimum position
    pair<int, int> tp;
    vector< vector<int> > posting(n);
    priority_queue<pair<int, int>, vector<pair<int, int> >, mycomparison > head;
    rep(i, n) cin >> tf[i];
    rep(i, n) {
        posting[i] = vector<int>(tf[i]);
        rep(j, tf[i]) {
            cin >> posting[i][j];
        }
    }
    rep(i, n) {
        head.push(make_pair(i, posting[i][0]));
        maxPos = max(maxPos, posting[i][0]);
    }
    len = INT_MAX;
    
    while (1) { //  while (posting[minTermIdx].size() > idx[minTermIdx]), segmentation fault!
        tp = head.top();
        minTermIdx = tp.first;
        minPos = tp.second;
        len = min(len, maxPos - minPos + 1);
        idx[minTermIdx]++;
        if (idx[minTermIdx] >= tf[minTermIdx]) break;
        if (posting[minTermIdx][idx[minTermIdx]] > maxPos) {
            maxPos = posting[minTermIdx][idx[minTermIdx]];
        }
        head.pop();
        head.push(make_pair(minTermIdx, posting[minTermIdx][idx[minTermIdx]]));
    }
    cout << len;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        searchsnippets();
        cout << endl;
    }
}
