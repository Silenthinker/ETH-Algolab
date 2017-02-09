// ALGOLAB BGL coin tossing
/*
 * Like soccer championship, formulate a max-flow problem
 * Pay attention to degenerate case where total scores on the boad are not equal to 
 * number of rounds 
 */


// Compile and run with one of the following:
// g++ -std=c++11 -O2 cointossing.cpp -o cointossing 

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
using namespace std;
using namespace boost;
// User definitions
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)


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
typedef graph_traits<Graph>::edge_iterator		        EdgeIt;		// to iterate over all edges


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

// function for individual testcase
void cointossing() {
    int n, m; cin >> n >> m; // # of players and rounds played
    int a, b, c; // player a, b, and result
    int s; // final score
    vector<int> scoreboard(n);
    // Create Graph and Maps
    int nVertices = 2+m+n;
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    rep(i,m) {
        // 0,...,m-1 are vertices of rounds
        // m,...m+n-1 are vertices of players
        // m+n and m+n+1 are source and sink
        cin >> a >> b>> c;
        // Add edges
        if(c==0) {
            eaG.addEdge(i, a+m, 1); // from, to, capacity
            eaG.addEdge(i, b+m, 1); // uncertain, simply assign capacity of 1
        }
        else if(c==1) {
            eaG.addEdge(i, a+m, 1);
            eaG.addEdge(i, b+m, 0);
        }
        else {
            eaG.addEdge(i, a+m, 0);
            eaG.addEdge(i, b+m, 1);
        }
    }
    // Add source and sink
    // Careful: The names 'source' and 'target' are already used for BGL's 
    // functions to get the two endpoints of an edge, use 'src' and 'sink'.
    Vertex src = add_vertex(G);
    Vertex sink = add_vertex(G);
    rep(i,m) {
        eaG.addEdge(src, i, 1); // all edges from source to the vertices have capacity of 1
    }
    rep(i,n) {
        cin >> s;
        scoreboard[i] = s;
        eaG.addEdge(i+m, sink, s); // all edges from the vertices to sink have capacity of the respective score of player
    }
    // First verify total scores equal # of rounds
    int sum = 0;
    rep(i,n) {
        sum += scoreboard[i];
    }
    if(sum!=m) {
        cout<<"no"<<endl;
        return;
    }
    // Calculate flow
    // If not called otherwise, the flow algorithm uses the interior properties
    // - edge_capacity, edge_reverse (read access),
    // - edge_residual_capacity (read and write access).
    
    long flow = push_relabel_max_flow(G, src, sink);
    //cout<<flow<<endl;
    if(flow==m) cout<<"yes"<<endl;
    else cout<<"no"<<endl;
    /*
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges.
        Vertex u = source(*ebeg, G);
        Vertex v = target(*ebeg, G);
        cout<<u<<"->"<<v<<": "<<capacitymap[*ebeg]<<endl;
    }
    */
}
// Main function to loop over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	rep(_,t) {
            cointossing();
        }
	return 0;
}
