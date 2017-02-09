// ALGOLAB BGL satellites
/* Algorithm
 * This is a minimum vertex cover problem.
 * In a bipartite graph, # of edges in a max matching is equal to 
 * # of nVertices in a min vertex cover.
 * In addition, turn max matching problem into a max flow problem.
 * Compute residual graph and find visited vertices with BFS from s.
 * Minimum vertex cover – all unvisited in L and all visited in R.
 */

// - MinCostMaxFlow with negative edge costs using cycle_canceling
// - MinCostMaxFlow with negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 satellites.cpp -o satellites; ./satellites

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
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

void satellites() {
    int g, s, l; // # of ground stations, satellites, and linkes
    cin >> g >> s >> l;
    
    // Create Graph and Maps
    const int nVertices = g+s+2;
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    // Add the edges
    // node index: stations from 0 to g-1; satellites from g to g+s-1;
    const int v_source = g+s;
    const int v_target = g+s+1;
    int g_node, s_node;
    // from station to satellites
    rep(_, l) {
        cin >> g_node >> s_node;
        eaG.addEdge(g_node, s_node+g, 1, 1); 
    }
    // from source to stations
    rep(i, g) {
        eaG.addEdge(v_source, i, 1, 0);
    }
    // from satellites to target
    rep(i, s) {
        eaG.addEdge(i+g, v_target, 1, 0);
    }
    
    // Run the algorithm
    // Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = find_flow_cost(G); // equal to flow
    
    // BFS to find vertex set S in which points are reachable from source in residual graph
    vector< int > vis(nVertices, false); // visited flags
    std::queue< int > stations; // stations to install software
    std::queue< int >  satellites; // satellites to install software
    std::queue< int > Q; // BFS queue (from std:: not boost::)
    vis[v_source] = true; // Mark the source as visited
    Q.push(v_source);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            Q.push(v);
        }
    }

    // Output 
    rep(v, nVertices) {
        if (!vis[v] && (0 <= v && v < g)) stations.push(v); // if stations
        if (vis[v] && (v >= g && v < g+s)) satellites.push(v-g); // if satellites
    }
    
    cout << stations.size() << " " << satellites.size() << endl;
    if (!(stations.empty() && satellites.empty())) {
        while (!stations.empty()) {
            cout << stations.front() << " ";
            stations.pop();
        }
        while (!satellites.empty()) {
            cout << satellites.front() << " ";
            satellites.pop();
        }
        cout << endl;
    }
}

int main() {
    int T; // # of test cases
    cin >> T;
    rep(_, T) {
        satellites();
    }
    return 0;
}

