// ALGOLAB BGL canteen
/* Algorithm
 * 
 */

// - MinCostMaxFlow with negative edge costs using cycle_canceling
// - MinCostMaxFlow with negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 canteen.cpp -o canteen; ./canteen

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <queue>
#include <climits>
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

void canteen() {
    int n; // # of days
    int a, c; // # of menus and cost per menu
    int s, p; // # of students and price per menu
    int v, e; // # of menus that can be stored and energy cost per menu
    int delta = 20; // make all costs positive
    cin >> n;
    
    // Create Graph and Maps
    const int nVertices = 2*n+2;
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    // Add the edges
    // node index: menu/student from 0 to n-1; 
    Vertex v_source = add_vertex(G);
    Vertex v_target = add_vertex(G);
    int nStudent = 0;
    
    
    rep(i, n) {
        cin >> a >> c;
        eaG.addEdge(v_source, i, a, c+delta); // from source to menu/student
    }
    
    rep(i, n) {
        cin >> s >> p;
        nStudent += s;
        eaG.addEdge(i, v_target, s, delta-p); // from student to target
    }
    
    rep(i, n-1) {
        cin >> v >> e;
        eaG.addEdge(i, i+1, v, e); // from menu of day i to menu of day i+1. DON'T ADD DELTA TO FREEZE EDGE!
    }
    
    // Run the algorithm
    // Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = find_flow_cost(G); // equal to flow
    int flow = 0;
    
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt ebeg, eend;
    for(tie(ebeg, eend) = out_edges(vertex(v_source,G), G); ebeg != eend; ++ebeg) {
        flow += capacitymap[*ebeg] - rescapacitymap[*ebeg];
    }
    
    if (flow == nStudent) cout << "possible ";
    else cout << "impossible ";
    cout << flow << " " << 2 * flow * delta - cost << endl;
    
}

int main() {
    ios_base::sync_with_stdio(false);
    int T; // # of test cases
    cin >> T;
    rep(_, T) {
        canteen();
    }
    return 0;
}

