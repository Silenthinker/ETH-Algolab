// ALGOLAB W4 graphs

// Compile and run with one of the following:
// g++ -std=c++11 -O2 ant.cpp -o ant; ./ant
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

//=========================
// TODO: solve WA problem
//=========================

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

void ant() {
    int n, e, s, a, b; cin>>n>>e>>s>>a>>b; // # of trees, edges, species, and start and end tree.
    Graph G(n);	// creates an empty graph on n vertices
    WeightMap weightmap = get(edge_weight, G);	// start by defining property maps for all interior properties
    int t1, t2, w, minw; // t1, t2: edge; w: time to travel along the edge
    int h; // hive
    rep(i,e) {
        cin>>t1>>t2;
        Edge e; bool success;
        Vertex u = t1; Vertex v = t2;
        tie(e, success) = add_edge(u, v, G);
        minw = INT_MAX;
        rep(_,s) {
            cin>>w;
            minw = w<minw ? w:minw; // obtain min time given an edge
        }
        weightmap[e] = minw;
    }
    rep(_,s) {
        cin>>h; // seems not useful...
    }
    
    // dijkstra_shortest_paths
    vector<Vertex> predmap(n);
    vector<Vertex> distmap(n);
    Vertex start = a;
    Vertex end = b;
    dijkstra_shortest_paths(G, 
                            start, 
                            predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).
                            distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G)))
                           );
    cout<<distmap[b]<<endl;
}

int main() {
    ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
    int t; cin>>t;
    rep(_,t)    ant();
    return 0;
    
}
