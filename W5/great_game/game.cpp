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

int n, m; // # of pos and transitions; global var for saving code

int round0(int pos, bool self, const vector< vector <int> >& tr) {
    if (pos==n) return 0;
    if (self) {
        int minround = INT_MAX;
        int rd;
        for (size_t i=0; i<tr[pos].size(); i++) {
            rd = 1+round0(tr[pos][i], false, tr);
            minround = minround>rd ? rd : minround;
        }
        return minround;
    }
    else {
        int maxround = INT_MIN;
        int rd;
        for (size_t i=0; i<tr[pos].size(); i++) {
            rd = 1+round0(tr[pos][i], true, tr);
            maxround = maxround<rd ? rd : maxround;
        }
        return maxround;
    }
}

void game0() {
    cin >> n >> m;
    int r, b; // pos of red and black meeples
    cin >> r >> b;
    vector< vector<int> > tr(n+1,vector<int>()); // u, v from 1 to n; tr[0] is never used.
    rep(i,m) {
        int u, v; cin >> u >> v;
        tr[u].push_back(v);
    }
    int H, M;
    H = round0(r,true,tr);
    M = round0(b,true,tr);
    if (H<M) cout<<0<<endl; // less rounds imply winning strategy
    else if (H>M) cout<<1<<endl;
    else {
        if (H%2==0) cout<<1<<endl;
        else cout<<0<<endl;
    }
}

int round1(int startpos, const vector< vector<int> >& tr) {
    /*
     * Fill round(pos, true/false) from right to left
     * round[i][1/0] -> from position i and move one's own meeple (1) or not (0), we need how
     * many rounds
    */
    vector< vector<int> > round(n+1, vector<int>(2));
    for (int i=n; i>=startpos; i--) {
        if (i==n) { // already at the end, no move needed!
            round[i][0] = 0;
            round[i][1] = 0;
        }
        else {
            int minround = INT_MAX;
            int maxround = INT_MIN;
            int rd;
            // case 1: one's own meeple 
            for (size_t j=0; j<tr[i].size(); j++) {
                rd = round[tr[i][j]][0];
                minround = minround>rd ? rd : minround;
            }
            round[i][1] = minround+1;
            // case 2: the other's meeple
            for (size_t j=0; j<tr[i].size(); j++) {
                rd = round[tr[i][j]][1];
                maxround = maxround<rd ? rd : maxround;
            }
            round[i][0] = maxround+1;
        }
    }
    return round[startpos][1];
}

void game1() {
    cin >> n >> m;
    int r, b; // pos of red and black meeples
    cin >> r >> b;
    vector< vector<int> > tr(n+1,vector<int>()); // u, v from 1 to n; tr[0] is never used.
    rep(i,m) {
        int u, v; cin >> u >> v;
        tr[u].push_back(v);
    }
    int H, M;
    H = round1(r,tr);
    M = round1(b,tr);
    if (H<M) cout<<0<<endl; // less rounds imply winning strategy
    else if (H>M) cout<<1<<endl;
    else {
        if (H%2==0) cout<<1<<endl;
        else cout<<0<<endl;
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t; // # of test cases
    rep(_,t) game1();
    return 0;
}