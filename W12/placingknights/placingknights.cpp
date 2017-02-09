// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating 
// - MinCostMaxFlow with negative edge costs using cycle_canceling
// - MinCostMaxFlow with negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
// Namespaces
using namespace boost;
using namespace std;
// User defined
#define forloop(i, l, h) for(int i = (l); i < (h); i++)
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

void placingknights() {
    int n; cin >> n; // length
    int nValidPos = 0;
    vector<vector<int> > cb(n, vector<int>(n));
    vector<pair<int, int> > offset{make_pair(-1, -2), make_pair(-1, 2), make_pair(1, -2), make_pair(1, 2), make_pair(-2, -1), make_pair(-2, 1), make_pair(2, -1), make_pair(2, 1)};
    rep(i, n) {
        rep(j, n) {
            cin >> cb[i][j];
            if (cb[i][j] == 1) nValidPos++;
        }
    }
    int nVertices = 2 + n*n;
    const int v_source = n*n, v_target = n*n + 1;
    
    // Create Graph and Maps
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    
    // Add the edges
    rep(i, n) {
        rep(j, n) {
            int v = i*n+j;
            if ((i % 2 == j % 2) && cb[i][j] == 1) {
                eaG.addEdge(v_source, v, 1);
                rep(k, offset.size()) {
                    int p = i + offset[k].first, q = j + offset[k].second;
                    if (p >= 0 && p < n && q >= 0 && q < n) {
                        if (cb[p][q] == 1) {
                            int u = p*n + q;
                            eaG.addEdge(v, u, 1);
                        }
                    }
                }
            }
        }
    }
    rep(i, n) {
        rep(j, n) {
            int u = i*n + j;
            if ((i % 2 + j % 2 == 1) && cb[i][j] == 1) {
                eaG.addEdge(u, v_target, 1);
            }
            
        }
    }
    /*
    // Print graph
    typedef graph_traits<Graph>::edge_iterator edge_iter;
    edge_iter ei, ei_end;
    for(tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        if (capacitymap[*ei] != 0) {
            cout << source(*ei, G) << "->" << target(*ei, G) << endl;
            cout<< capacitymap[*ei] << ", " << weightmap[*ei] << endl;
        }
    }
    cout << endl;
    */
    
    // Run the algorithm
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    long flow = push_relabel_max_flow(G, v_source, v_target);
    //cout << "flow = " << flow << endl;
    cout << nValidPos - flow << "\n";
    
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t; // # of test cases
    rep(_, t) placingknights();
    return 0;
}
