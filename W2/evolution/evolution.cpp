/* Find oldest ancestor which is not older than the specified age
 * 
 * g++ -Wall -O0 -g -o evolution evolution.cpp
 */

#include <iostream>
#include <vector>
#include <algorithm> // set_difference, sort
#include <string> // string
#include <map> // ordered map
#include <utility> // pair
#include <queue> // priority queue
#include <functional> // greater

using namespace std;

#define forloop(i, l, h) for (int i=(l); i< (h); i++)
#define rep(i, N) forloop(i, 0, N)

void evolution0() {
    int n, q, a, maxAge = 0, rootIdx; // # of species and queries; age; max age
    cin >> n >> q;
    string s, p;
    vector<int> ancestor(n, -1); // -1 means that root does not have ancestor
    vector<int> age(n);
    map<string, int> name2int;
    vector<string> int2name(n);
    vector<pair<int, int> > queries(q); // name, age
    rep(i, n) {
        cin >> s >> a;
        int2name[i] = s;
        name2int[s] = i;
        age[i] = a;
        if (maxAge < a) {
            maxAge = a;
            rootIdx = i; // oldest species is its own ancestor
        }
    }
    rep(_, n-1) {
        cin >> s >> p;
        ancestor[name2int[s]] = name2int[p];
    }
    ancestor[rootIdx] = rootIdx;
    rep(i, q) {
        cin >> s >> a;
        queries[i] = make_pair(name2int[s], a);
    }
    
    rep(i, q) {
        int species = queries[i].first; maxAge = queries[i].second;
        while (1) {
            a = age[ancestor[species]];
            if (a > maxAge || ancestor[species] == species) break;
            species = ancestor[species];
        }
        cout << int2name[species] << " ";
    }
}

/* Helper function that performs each query
 */
int query(vector<int> ancestor, vector<int> age, int s, int maxAge) {
    int species = s;
    while (1) {
        int a = age[ancestor[species]];
        if (a > maxAge || ancestor[species] == species) break;
        species = ancestor[species];
    }
    return species;
}
void evolution() {
    int n, q, a, maxAge = 0, rootIdx; // # of species and queries; age; max age
    cin >> n >> q;
    string s, p;
    vector<int> ancestor(n, -1); // -1 means that root does not have ancestor
    vector<int> age(n);
    map<string, int> name2int;
    vector<string> int2name(n);
    vector<priority_queue<int, vector<int>, greater<int> > > ppqueries(n); // preprocessed queries
    map<pair<int, int>, int > result;
    vector<pair<int, int> > queries(q);
    rep(i, n) {
        cin >> s >> a;
        int2name[i] = s;
        name2int[s] = i;
        age[i] = a;
        if (maxAge < a) {
            maxAge = a;
            rootIdx = i; // oldest species is its own ancestor
        }
    }
    rep(_, n-1) {
        cin >> s >> p;
        ancestor[name2int[s]] = name2int[p];
    }
    ancestor[rootIdx] = rootIdx;
    rep(i, q) {
        cin >> s >> a;
        result[make_pair(name2int[s], a)] = -1;
        queries[i] = make_pair(name2int[s], a);
        ppqueries[name2int[s]].push(a);
    }
    
    rep(i, n) {
        int species = i;
        while (!ppqueries[i].empty()) {// if query of ith species exists...
            int maxAge = ppqueries[i].top(); ppqueries[i].pop();
            int grandfather = query(ancestor, age, species, maxAge);
            result[make_pair(i, maxAge)] = grandfather;
            species = grandfather;
        }
    }
    
    rep(i, q) {
        cout << int2name[result[queries[i]]] << " ";
    }
    
    
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin >> t;
    rep(_, t) {
        evolution();
        cout << endl;
    }
}
