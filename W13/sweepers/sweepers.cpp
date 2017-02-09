// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 sweepers.cpp -o sweepers ./flows


// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
using namespace std;
using namespace boost;
// User defined
#define forloop(i, l, h) for (int i = (l); i < (h); i ++)
#define rep(i, N) forloop(i, 0, N)

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
	property<edge_capacity_t, long,
		property<edge_residual_capacity_t, long,
			property<edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	property_map<Graph, edge_capacity_t>::type		EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type		ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	graph_traits<Graph>::edge_descriptor			Edge;
typedef	graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap	&capacitymap;
    ReverseEdgeMap	&revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
            G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
            Edge e, reverseE;
            bool success;
            tie(e, success) = add_edge(from, to, G);
            tie(reverseE, success) = add_edge(to, from, G);
            capacitymap[e] = capacity;
            capacitymap[reverseE] = 0;
            revedgemap[e] = reverseE;
            revedgemap[reverseE] = e;
    }
};

void dfs(Graph &G, int start, vector<bool> &visited) {
    OutEdgeIt oebeg, oeend;
    vector<int> stack;
    stack.push_back(start); visited[start] = true;
    while (!stack.empty()) {
        int u = stack.back(); stack.pop_back();
        for (tie(oebeg, oeend) = out_edges(u, G); oebeg != oeend; ++oebeg) {
            const int v = target(*oebeg, G);
            if (!visited[v]) {
                visited[v] = true;
                stack.push_back(v);
            }
        }
    }
}

void sweepers() {
    int n, m, s; cin >> n >> m >> s; // # of rooms, corridors, sweeps
    const int nVertices = n + 2;
    map<int, int> locSweepers, doors;
    vector<int> degrees(nVertices, 0);
    vector<pair<int, int> > corridors(m);
    rep(i, s) {
        int loc; cin >> loc;
        if (locSweepers.find(loc) != locSweepers.end()) locSweepers[loc]++;
        else locSweepers[loc] = 1;
        degrees[loc]++;
    }
    
    rep(i, s) {
        int loc; cin >> loc;
        if (doors.find(loc) != doors.end()) doors[loc]++;
        else doors[loc] = 1;
        degrees[loc]++;
    }
    
    rep(i, m) {
        int l, r; cin >> l >> r;
        corridors[i] = make_pair(l, r);
        //cout << l << "->" << r << endl;
        degrees[l]++;
        degrees[r]++;
    }
    
    // build graph
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    rep(i, m) {
        eaG.addEdge(corridors[i].first, corridors[i].second, 1);
        eaG.addEdge(corridors[i].second, corridors[i].first, 1);
    }
    const int v_source = add_vertex(G), v_target = add_vertex(G);
    for (map<int, int>::iterator it = locSweepers.begin(); it != locSweepers.end(); ++it) {
        //cout << it->first << ", " << it->second<< endl;
        eaG.addEdge(v_source, it->first, it->second);
    }
    for (map<int, int>::iterator it = doors.begin(); it != doors.end(); ++it) {
        eaG.addEdge(it->first, v_target, it->second);
    }
    
    
    // case: s = 0, no corridors
    if (s == 0){
        if (m == 0) cout << "yes";
        else cout << "no";
    }
    else {
        // case s >= 1, every vertex can be reachable from sweeper and every vertex has even degree, every sweeper is able to exit; bordercase: single isolated vertex is allowed even if it can be reached
        rep(i, nVertices) {
            //cout << i << "->degree" << degrees[i] << endl;
            if (degrees[i]%2 != 0) {
                cout << "no";
                return;
            }
        }
        
        // problematic:
        // if the room has no corridors attached, we assume it is cleaned (only corridors need cleaning,
        // no corridor, no cleaning needed).
        vector<bool> visited(nVertices, false);
        for (map<int, int>::iterator it = locSweepers.begin(); it != locSweepers.end(); ++it) {
            if (!visited[it->first]) dfs(G, it->first, visited);
        }
        rep(i, nVertices) {
            if (!visited[i] && i < n && (degrees[i]!=0)){
                cout << "no"; 
                return;
            }
        }
        long flow = push_relabel_max_flow(G, v_source, v_target);
        if (flow == s) cout << "yes";
        else cout << "no";
        return;
    }
}

// Main function to loop over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T; cin >> T;
    for (; T > 0; --T)	{
        sweepers();
        cout << "\n";
    }
    return 0;
}
