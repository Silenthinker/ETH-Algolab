// ALGOLAB W4 graphs

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bridge.cpp -o bridge; ./bridge
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets


// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <cassert>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/biconnected_components.hpp>

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


void bridge() {
    int n, m; cin>>n>>m; // # of islands and bridges
    int e1, e2;
    Edge e; bool success;
    EdgeIt ebeg, eend; // Edge iterator
    Graph G(n);
    rep(i,m) {
        cin>>e1>>e2; // e1 is connected to e2 by i-th bridge
        Vertex u = e1; Vertex v = e2;
        tie(e, success) = add_edge(u, v, G);
    }
    
    WeightMap wm = get(edge_weight, G); 
    int ncomp = biconnected_components(G, wm); 
    set<pair<int,int> > res; // vector to store result as pair<u,v> (u<v)
    
    EdgeIt ei, ei_end;
    vector<int> count(ncomp,0);// see each component has how many edges
    for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei)
        count[wm[*ei]]++;
    for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei){
        if(count[wm[*ei]]==1){
            int u=source(*ei,G), v=target(*ei,G);
            pair<int,int> bridge = (u<v ? make_pair(u,v) : make_pair(v,u));
             res.insert(bridge);
        }
    }// go over all edges and see the size of each bi-component
    cout << res.size() << endl; // res is already sorted !
    // output result
    for(set<pair<int,int> >::iterator it=res.begin(); it!=res.end(); ++it) 
        cout << it->first << " " << it->second << endl;
}


int main() {
    int c; cin>>c;
    rep(_, c)   bridge();
    return 0;    
}