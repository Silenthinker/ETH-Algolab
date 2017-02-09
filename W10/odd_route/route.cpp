/*
 * 
 * run with 
 * g++ -std=c++11 -O2 route.cpp -o route
 * 
 */

// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <queue> // priority queue
#include <utility> // pair
#include <unordered_set>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// User defined
#define forloop(i, l, h) for(int i = (l); i < (h); i++)
#define rep(i, N) forloop(i, 0, N)

// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int,
                    property<edge_capacity_t, int> > 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef property_map<Graph, edge_capacity_t>::type      CapcityMap;

struct MyComparator {
    bool operator() (const pair<Edge, int> &lhs, const pair<Edge, int> &rhs) {
        // pair<Vertex, distance of Vertex from the source of graph>
        return lhs.second > rhs.second;
    }
}; 

void route() {
    int n, m, s, t, si, ti, wi; // # of vertices and edges
    cin >> n >> m >> s >> t;
    // Create graph
    Graph G(n);
    WeightMap weightmap = get(edge_weight, G);
    CapcityMap edge2int = get(edge_capacity, G);
    int edgeIndex = 0;
    std::unordered_set<int> unvisited;
    rep(_, m) {
        Edge e; bool success;
        cin >> si >> ti >> wi;
        tie(e, success) = add_edge(si, ti, G);
        weightmap[e] = wi;
        edge2int[e] = edgeIndex;
        unvisited.insert(edgeIndex);
        ++edgeIndex;
    }
    /*
    // print graph
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        cout << source(*ebeg, G) << "->" << target(*ebeg, G) << ": " << weightmap[*ebeg] << endl;
    }
    */
    // Try BFS
    priority_queue<pair<Edge, int>, vector< pair<Edge, int> >, MyComparator> pq;
    OutEdgeIt oebeg, oeend;
    for (tie(oebeg, oeend) = out_edges(s, G); oebeg != oeend; ++ oebeg) {
        pq.push(make_pair(*oebeg, weightmap[*oebeg]));
        //cout << weightmap[*oebeg] << endl;;
    }
    
    int dist = 0;
    while (!unvisited.empty() && !pq.empty()) {
        Edge e;
        tie(e, dist) = pq.top();
        //cout << source(e, G) << "->" << target(e, G) << ", dist = " << dist << endl;
        //cout << weightmap[e] << endl;
        pq.pop(); unvisited.erase(edge2int[e]);
        Vertex v = target(e, G);
        if (v == t && dist%2 == 1) break; 
        for (tie(oebeg, oeend) = out_edges(v, G); oebeg != oeend; ++oebeg) {
            pq.push(make_pair(*oebeg, dist + weightmap[*oebeg]));
        }
    }
    if (dist%2 == 1) cout << dist;
    else cout << "no";
    cout << "\n";
    
    
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; // # of test cases
    cin >> t;
    rep(_, t) route();
    return 0;
}