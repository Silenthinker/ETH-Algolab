// STL includes
#include <iostream>
#include <vector>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Namespaces
using namespace boost;
using namespace std;

// User's definitions
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long> > > > > Graph;
// Interior Property Maps
typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type       EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator
typedef graph_traits<Graph>::edge_iterator EdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE; 
        revedgemap[reverseE] = e; 
    }
};

// Too slow
void bonus_0() {
    int n; cin >> n; // # of shelves
    vector<int> coins(n*n);
    rep(i, n*n) {
        cin >> coins[i];
    }
    int nVertices = 2*n*n+2;
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    const int v_source = 2*n*n, v_target = 2*n*n+1;
    // Construct graph
    rep(i, n*n) {
        eaG.addEdge(i, i+n*n, 1, -1*coins[i]);
        eaG.addEdge(i, i+n*n, 1, 0);
    }
    rep(i, n*n-1) {
        if ((i+1)%n != 0) { // if not right border
            //eaG.addEdge(i, i+1, 1, 0);
            eaG.addEdge(i+n*n, i+1, 1, 0);
        }
        if (i < (n-1)*n) {
            //eaG.addEdge(i, i+n, 1, 0);
            eaG.addEdge(i+n*n, i+n, 1, 0);
        }
    }
    eaG.addEdge(v_source, 0, 2, 0);
    //eaG.addEdge(n*n-1, v_target, 1, 0);
    eaG.addEdge(2*n*n-1, v_target, 2, 0);
    // max flow min cost
    int flow = push_relabel_max_flow(G, v_source , v_target );
    cycle_canceling(G);
    int cost = find_flow_cost (G);
    cout << -1*cost << "\n";
    /*
    EdgeIt ebeg, eend;
    int nBonus = 0;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        Vertex u = source(*ebeg, G), v = target(*ebeg, G);
        if (u == v - n*n) {
            if (rescapacitymap[*ebeg] == 0) nBonus++;
        }
    }
    cout << "nBonus = " << nBonus << endl;
    */
    // print graph
    /*
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        cout << source(*ebeg, G) << "->" << target(*ebeg, G) << ": " << capacitymap[*ebeg] << ", " << weightmap[*ebeg] << endl;
    }
    */
}

void bonus() {
    int n; cin >> n; // # of shelves
    vector<int> coins(n*n);
    rep(i, n*n) {
        cin >> coins[i];
    }
    int nVertices = 2*n*n+2;
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    const int v_source = 2*n*n, v_target = 2*n*n+1;
    // Construct graph
    int delta = 100;
    rep(i, n*n) {
        eaG.addEdge(i, i+n*n, 1, delta-coins[i]);
        eaG.addEdge(i, i+n*n, 1, delta);
    }
    rep(i, n*n-1) {
        if ((i+1)%n != 0) { // if not right border
            eaG.addEdge(i+n*n, i+1, 1, 0);
        }
        if (i < (n-1)*n) {
            eaG.addEdge(i+n*n, i+n, 1, 0);
        }
    }
    eaG.addEdge(v_source, 0, 2, 0);
    eaG.addEdge(2*n*n-1, v_target, 2, 0);
    // max flow min cost
    successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = find_flow_cost (G);
    
    EdgeIt ebeg, eend;
    int nBonus = 0;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        Vertex u = source(*ebeg, G), v = target(*ebeg, G);
        if (u == v - n*n) {
            if (rescapacitymap[*ebeg] == 0) nBonus++;
        }
    }
    //cout << "nBonus = " << nBonus << endl;
    cout << -1*cost + nBonus * delta<< "\n";
    
    // print graph
    /*
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        if (capacitymap[*ebeg] != 0) {
            cout << source(*ebeg, G) << "->" << target(*ebeg, G) << ": " << capacitymap[*ebeg] << ", " << weightmap[*ebeg] << endl;
        }
    }
    */
}

int main() {
    int t; cin >> t; // # of test cases
    rep(_, t) bonus();
    return 0;
}



