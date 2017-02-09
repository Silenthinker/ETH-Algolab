// STL
#include <iostream>
#include <cmath>
#include <climits>
#include <queue> // std::priority_queue
#include <vector>
#include <algorithm> // std::lower_bound, sort
#include <utility>

using namespace std;

// Predefinition
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

bool myfunction (pair<int, int> &lhs, pair<int, int> &rhs) {
    return (lhs.second < rhs.second);
}

bool myfunction_lower_bound (pair<int, int> &lhs, int rhs) {
    return (lhs.second < rhs);
}

struct MyCmpStruct
{
    bool operator() (const pair<int, int> &lhs, const pair<int, int> &rhs){
        return (lhs.second < rhs.second);
    }
} myobject;

void museum0() {
    // Naive brute force
    int N,M; // # of switches and rooms
    cin>>N>>M;
    vector<int> target(M);
    rep(i,M) {
        cin>>target[i];
    }
    vector< vector< vector<int> > > onoff(N,vector< vector<int> >(M,vector<int>(2)));
    rep(i,N) {
        rep(j,M) {
            cin>>onoff[i][j][0]>>onoff[i][j][1]; // [0] for on and [1] for off
        }
    }
    vector<int> output(M,0);
    rep(i,N) {
        rep(j,M) {
            output[j] += onoff[i][j][0];
        }
    }
    // Brute force
    int opttimes = INT_MAX;
    for(int s = 0; s <1<<N; ++s) {
        int times = 0;
        vector<int> tempoutput = output;
        for(int i = 0; i<N; ++i) {
            if(s & 1<<i) { // find which switch is to be changed
                times++;
                rep(j,M) {
                    tempoutput[j] = tempoutput[j]-onoff[i][j][0]+onoff[i][j][1];
                }
            }
        }
        int nsuccess = 0;
        rep(k,M) {
            if (tempoutput[k]==target[k]) nsuccess++;
        }
        if (nsuccess==M) {
            opttimes = opttimes>times ? times:opttimes;
        }
    }
    if (opttimes==INT_MAX) cout<<"impossible"<<endl;
    else cout<<opttimes<<endl;
}

void museum1() {
    // Implement split and list approach
    int N,M; // # of switches and rooms
    cin>>N>>M;
    vector<int> target(M);
    rep(i,M) {
        cin>>target[i];
    }
    vector< vector< vector<int> > > onoff(N,vector< vector<int> >(M,vector<int>(2)));
    rep(i,N) {
        rep(j,M) {
            cin>>onoff[i][j][0]>>onoff[i][j][1]; // [0] for on and [1] for off
        }
    }
    
    /* Split */
    // list 1
    vector< pair<int,vector<int> > > list1, list2;
    vector<int> output1(M,0);
    rep(i,N/2) {
        rep(j,M) {
            output1[j] += onoff[i][j][0];
        }
    }
    for (int s = 0; s < 1 << N/2; ++s) {
        int times = 0;
        vector<int> tempoutput1 = output1;
        for(int i = 0; i<N/2; ++i) {
            if(s & 1<<i) { // find which switch is to be changed
                times++;
                rep(j,M) {
                    tempoutput1[j] = tempoutput1[j]-onoff[i][j][0]+onoff[i][j][1];
                }
            }
        }
        list1.push_back(make_pair(times,tempoutput1));
    }
    //list 2
    vector<int> output2(M,0);
    for (int i=0;i<(N-N/2);i++) {
        rep(j,M) {
            output2[j] += onoff[i+N/2][j][0];
        }
    }
    int length = 1<<(N-N/2);
    for (int s = 0; s < length; ++s) {
        int times = 0;
        vector<int> tempoutput2 = output2;
        for(int i = 0; i<(N-N/2); ++i) {
            if(s & 1<<i) { // find which switch is to be changed
                times++;
                rep(j,M) {
                    tempoutput2[j] = tempoutput2[j]-onoff[i+N/2][j][0]+onoff[i+N/2][j][1];
                }
            }
        }
        list2.push_back(make_pair(times,tempoutput2));
    }

    
    int opttimes = INT_MAX;
    for (size_t i=0;i<list1.size();i++) {
        for (size_t j=0;j<list2.size();j++) {
            int times = 0;
            int nsuccess = 0;
            rep(k,M) {
                if (list1[i].second[k]+list2[j].second[k] == target[k]) {
                    nsuccess++;
                }
            }
            if (nsuccess==M) {
                times = list1[i].first+list2[j].first;
                opttimes = opttimes>times ? times:opttimes;
            }
        }
    }
    
    if (opttimes==INT_MAX) cout<<"impossible"<<endl;
    else cout<<opttimes<<endl;
}

void getList(vector< vector< vector<int> > >& control, int M, vector< vector<int> >& list, vector<int>& nFlicks) {
    int N = control.size(), nFlick;
    for(int s = 0; s < 1<<N; s++) {
        vector<int> sum(M, 0);
        nFlick = 0;
        for(int j = 0; j < M; j++) {
            for(int i = 0; i < N; i++) {
                if(s & 1<<i) {
                    sum[j] += control[i][j][1];
                    nFlick++;
                }
                else sum[j] += control[i][j][0];
            }
        }
        list.push_back(sum);
        nFlicks.push_back(nFlick / M);
    }
}

/* Binary search to find upper bound of val in vec
 * return index
 * for example vec = 3, 3, 5 and val = 3,
 * should return 0
 * if no upper bound, return vec.size()
 */
int binarySearch(vector<pair<int, int> >& vec, int val) {
    int l = 0, h = vec.size()-1, m;
    while (l != h) {
        m = (l + h) / 2;
        if (vec[m].second >= val) h = m;
        else l = m+1;
    }
    if (vec[l].second < val) return vec.size();
    else return l;
}

void museum2() {
    /* Implement split and list approach
     * 
     * First for a given room, list all possible combinations
     * then check for the remaining rooms if some combination
     * is valid.
     */
    
    int N, M; // # of switches and rooms
    cin >> N >> M;
    vector<int> targets(M);
    rep(i, M) {
        cin >> targets[i];
    }
    vector< vector< vector<int> > > control_0(N/2, vector< vector<int> >(M, vector<int>(2))), control_1(N - N/2, vector< vector<int> >(M, vector<int>(2))); // [0] for on and [1] for off
    // Split
    rep(i, N/2) {
        rep(j, M) {
            cin >> control_0[i][j][0] >> control_0[i][j][1];
        }
    }
    rep(i, N-N/2) {
        rep(j, M) {
            cin >> control_1[i][j][0] >> control_1[i][j][1];
        }
    }
    vector< vector<int> > list_0, list_1;    
    vector<int> nFlick_0, nFlick_1;
    getList(control_0, M, list_0, nFlick_0); // list_0[i][j] -> jth room light in ith subset
    getList(control_1, M, list_1, nFlick_1);
    vector< pair<int, int> > transpose(list_0.size(), pair<int, int>()); // transpose[i] -> ith possible subset; list_0 smaller than list_1
    rep(i, list_0.size()) {
        transpose[i] = make_pair(i, list_0[i][0]);
    }
    
    sort(transpose.begin(), transpose.end(), myfunction);
    int nSuccess, leastFlick = INT_MAX;
    
    /*
    rep(i, list_0.size()) {
        rep(j, M) {
            cout << list_0[i][j] << " ";
        }
        cout << endl;
        cout << "nFlick = " << nFlick_0[i] << endl;
    }
    cout << endl;
    rep(i, list_1.size()) {
        rep(j, M) {
            cout << list_1[i][j] << " ";
        }
        cout << endl;
        cout << "nFlick = " << nFlick_1[i] << endl;
    }
    cout << endl;
    
    rep(i, list_0.size()) {
        cout << transpose[i].first << "->" << transpose[i].second << endl;
    }
    cout << endl;  
    int idx = binarySearch(transpose, 0);
    cout << transpose[idx].first << "->" << transpose[idx].second << endl;
    cout << endl;
    
    cout << endl;
    */
    rep(i, list_1.size()) {
        int rem = targets[0] - list_1[i][0];
        int idx = binarySearch(transpose, rem);
        //cout << "target = " << targets[0] << ", " << list_1[i][0] << endl;
        //cout << "rem = " << rem << ", transpose = " << transpose[idx].first << "->" << transpose[idx].second << endl;
        while (idx != transpose.size()) {
            if (transpose[idx].second > rem) break;
            nSuccess = 1;
            forloop(j, 1, M) {
                if (list_1[i][j] + list_0[transpose[idx].first][j] == targets[j]) nSuccess++;
            }
            if (nSuccess == M) {
                leastFlick = min(leastFlick, nFlick_0[transpose[idx].first] + nFlick_1[i]);
            }
            idx++;
        }
    }
    
    if (leastFlick == INT_MAX) cout << "impossible" << "\n";
    else cout << leastFlick << "\n";
    

    
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin>>t;
    rep(_,t) {
        museum2();
    }
    return 0;
}