// ALGOLAB W4 graphs

// Compile and run with one of the following:
// g++ -std=c++11 -O2 buddies.cpp -o buddies; ./buddies < sample.in
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

//=========================
// Use max_cardinality_matching to solve the problem
//=========================

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <string>
#include <unordered_map>
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
		no_property 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
// typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void buddies() {
    int n, c, f; // # of students, characteristics, min # of original matching
    cin>>n>>c>>f;
    std::unordered_map<string, int> str2int; // map string to integer
    vector<vector<int> > chr(n, vector<int>(c)); // student characteristics
    int sizeMap = 0;
    rep(i, n) {
        rep(j, c) {
            string ch; // single characteristic
            cin>>ch;
            if (str2int.find(ch) == str2int.end()) {
                str2int[ch] = (++sizeMap);
            }
            chr[i][j] = str2int[ch];
        }
        std::sort(chr[i].begin(), chr[i].end());
    }
    // Build graph
    Graph G(n);
    
    rep(i, n) {
        forloop(j, i+1, n){
            vector<int> intersection;
            set_intersection(chr[i].begin(), 
                             chr[i].end(), 
                             chr[j].begin(), 
                             chr[j].end(),
                             inserter(intersection, intersection.end()) // need inserter
                            );
            //cout<<i<<"--"<<j<<" : "<<intersection.size()<<endl;;
            if(intersection.size()>f) {
                add_edge(i, j, G);
            }
        }
    }
    
    // Matching!
    vector<Vertex> mate(n);
    // find the maximum cardinality matching. we'll use a checked version
    // of the algorithm, which takes a little longer than the unchecked
    // version, but has the advantage that it will return "false" if the
    // matching returned is not actually a maximum cardinality matching
    // in the graph.
    edmonds_maximum_cardinality_matching(G, &mate[0]);
    if(matching_size(G, &mate[0])<n/2) {
        cout<<"optimal"<<endl;
    }
    else {
        cout<<"not optimal"<<endl;
    }    
}

int main() {
    ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
    int t; cin>>t;
    rep(_,t)    buddies();
    return 0;
}