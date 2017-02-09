// ALGOLAB BGL real_estate
// - MinCostMaxFlow with negative edge costs using cycle_canceling
// - MinCostMaxFlow with negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 real_estate.cpp -o real_estate; ./real_estate

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
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

void real_estate() {
    int N, M, S; // # of buyers, sites, and states
    cin >> N >> M >> S;
    vector< long > l(S); // limit on # of sites that can be sold
    vector< long > s(M); // to which state each site belongs to
    vector< vector< long > > b(N, vector< long >(M)); // bids, suposed to be integer
    rep(i, S) cin >> l[i];
    rep(i, M) cin >> s[i]; // states from 0 to S-1
    long maxBid = 100;
    rep(i, N) {
        rep(j, M) {
            cin >> b[i][j];
        }
    }
    
    // Create Graph and Maps
    const int nVertices = N + M + S + 2;
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    // Add the edges
    // buyers: 0 to N-1; sites: N to M+N-1; states: M+N to M+N+S-1; 
    //source: M+N+S; target: M+N+S+1 
    const int v_source = M+N+S;
    const int v_target = M+N+S+1;
    // buyers to sites
    rep(i, N) {
        rep(j, M) {
            eaG.addEdge(i, j+N, 1, maxBid - b[i][j]); // make cost positive
        }
    }
    // source to buyers 
    rep(i, N) {
        eaG.addEdge(v_source, i, 1, 0);
    }
    // sites to states
    rep(i, M) {
        eaG.addEdge(i+N, s[i]+M+N-1, 1, 0);
    }
    // states to target
    rep(i, S) {
        eaG.addEdge(M+N+i, v_target, l[i], 0);
    }
    // Run the algorithm

    // Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = find_flow_cost(G);
    int flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(tie(e, eend) = out_edges(vertex(v_source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    
    /*
    typedef graph_traits<Graph>::edge_iterator edge_iter;
    edge_iter ei, ei_end;
    for(tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        cout << source(*ei, G) << "->" << target(*ei, G) << endl;
        cout<< capacitymap[*ei] << ", " << weightmap[*ei] << endl;
    }
    */
    
    // cost = -1*profit+flow*maxBid
    cout << flow << " " << (flow*maxBid-cost) << endl;
//     cout << "-----------------------" << endl;
//     cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << endl;
//     cout << "flow " << flow2 << endl; // 5
//     cout << "cost " << cost2 << endl; // 12
//     cout << "-----------------------" << endl;

}

int main() {
    int T; // # of test cases
    cin >> T;
    rep(_, T) {
        real_estate();
    }
    return 0;
}

