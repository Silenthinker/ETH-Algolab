// ALGOLAB W4 graphs

// Compile and run with one of the following:
// g++ -std=c++11 -O2 graphs.cpp -o graphs; ./graphs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// Define
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void graphs() {
    // Create Graph, Vertices and Edges
    // ================================
    int n, m; cin>>n>>m;// # of vertices and edges of the graph
    int s, t, w; // # starting point, ending point and weight of edge
    Graph G(n);	// creates an empty graph on n vertices
    WeightMap weightmap = get(edge_weight, G);	// start by defining property maps for all interior properties
    rep(_,m) {
        cin>>s>>t>>w;
        Edge e;	bool success;
        Vertex u = s;
        Vertex v = t;
        tie(e, success) = add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
                                                // Caveat: if u or v don't exist in G, G is automatically extended!
        weightmap[e] = w;			// Otherwise it is false in case of failure when the edge is a duplicate
        assert(source(e, G) == u && target(e, G) == v);	// This shows how to get the vertices of an edge
    }
    
    // Prim minimum spanning tree
    // ==========================
    vector<Vertex> primpredmap(n);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
    Vertex start = 0;
    prim_minimum_spanning_tree(G, make_iterator_property_map(primpredmap.begin(), get(vertex_index, G)),	// Prim from user-defined start,
        root_vertex(start));									// old primpredmap gets reset first!	
    int totalweight = 0;
    rep(i,n) {
        if (primpredmap[i] != i) {
            Edge e; bool success;
            tie(e, success) = edge(i, primpredmap[i], G);
            totalweight += weightmap[e];
        }
    }
    
    // Dijkstra shortest paths
    // =======================
    vector<Vertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    vector<int> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    start = 0;
    dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
        predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
        distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter
    int maxdist = 0;
    Vertex furthest = start;
    int reachable = 0;
    rep(i,n) {
        if (distmap[i] < INT_MAX) {	// check whether vertex i is reachable, i.e. has "finite" distance from the start
            ++reachable;		// INT_MAX comes from climits and the type of distmap (vector<int>).
            if (distmap[i] > maxdist) {
                    maxdist = distmap[i];
                    furthest = i;
            }
        }
    }
    cout<<totalweight<<" "<<maxdist<<endl;
}

int main() {
    ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
    int t; cin>>t;
    rep(_,t)    graphs();
    return 0;
    
}



