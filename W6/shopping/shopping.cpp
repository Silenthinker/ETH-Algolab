// ALGOLAB BGL shopping
/*
 * Consider flow as the # of times passing the edge
 * from home to every shop, passed streets should be different, leading to every street having 
 * capacity equal to 1
 * From every shop, there is a conceptual street to a sink, whose capacity equals # of 
 * stores at each Location
 * ATTENTION: problem is undirected, turn it into directed case!
 */


// Compile and run with one of the following:
// g++ -std=c++11 -O2 shopping.cpp -o shopping 

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
void shopping() {
    int n, m, s; // # of vertics, edges, stores
    cin >> n >> m >> s;
    vector<int> storeLocation(s);
    int start, end; // two end pts of each street
    // Create Graph and Maps
    Graph G(n+1);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    rep(i,s) cin >> storeLocation[i];
    rep(i,m) {
        cin >> start >> end;
        eaG.addEdge(start, end, 1); // from, to, capacity; one street can only be passed once
        eaG.addEdge(end, start, 1);
    }
    // Add source and sink
    // Careful: The names 'source' and 'target' are already used for BGL's 
    // functions to get the two endpoints of an edge, use 'src' and 'sink'.
    Vertex src = 0; // source is 0
    Vertex sink = add_vertex(G); 
    rep(i,s) {
        // There might be more than one store at an intersection!
        if(!edge(storeLocation[i], sink, G).second) { // if not found
            eaG.addEdge(storeLocation[i], sink, 1); 
        }
        else { // if more than one store
            eaG.addEdge(storeLocation[i], sink, 
                        capacitymap[edge(storeLocation[i], sink, G).first]+1);
        }
        //cout<<capacitymap[edge(storeLocation[i], sink, G).first]<<endl;
    }
    
    // Calculate flow
    // If not called otherwise, the flow algorithm uses the interior properties
    // - edge_capacity, edge_reverse (read access),
    // - edge_residual_capacity (read and write access).
    
    long flow = push_relabel_max_flow(G, src, sink);
    //cout<<flow<<endl;
    if(flow==s) cout<<"yes"<<endl;
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
            shopping();
        }
	return 0;
}
