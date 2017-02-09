// ALGOLAB BGL shopping
/* we basically compute a circulation in the network G',
 * where e has capacity  c(e) - l(e), and a node v has
 * demand dv - L(v).
 * L(v) =  f_0^in (v) - f_0^out (v) 
 * =  \sum_{e into v} l(e) - \sum_{v out of v} l(e).
 * the circulation is feasible if and only if the maxflow
 * has value exactly \sum_{v demand node} (d+f_0^out(v)) since sometimes sum g >= sum d.
 * f(s,v) = g+f_0^in(v)
 * f(v,t) = d+f_0^out(v)
 */


// Compile and run with one of the following:
// g++ -std=c++11 -O2 kindom.cpp -o kindom 

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
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
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex


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
void kindom() {
    int l, q; // # of locations and paths
    cin >> l >> q;
    int nVertices = l+2; // l vertices plus two sinks and two sources
    int g, d; // # of soldies stationed at location i and demand
    int f, t, c, C;
    // Create Graph and Maps
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    Vertex src = l; Vertex sink = l+1;
    vector<Vertex> inMinCapSum(l,0), outMinCapSum(l,0);
    int totalSoldiers = 0;
    int totalReq = 0;
    // Add edges between vertices and source and sink
    rep(i,l) {
        cin >> g >> d;
        eaG.addEdge(src, i, g); // from, to, capacity
        eaG.addEdge(i, sink, d);
        totalSoldiers += g;
        totalReq += d;
    }
    // Add edges between vertices 
    rep(i,q) {
        cin >> f >> t >> c >> C;
        eaG.addEdge(f, t, C-c); // scale C to C-c
        inMinCapSum[t] += c; 
        outMinCapSum[f] += c;
    }
    // Adjust edges capacity between vertices and source and sink
    rep(i,l) {
        if(edge(src,i,G).second) {
            capacitymap[edge(src,i,G).first] += inMinCapSum[i];
            capacitymap[edge(i,sink,G).first] +=outMinCapSum[i];
        }
    }
    // Calculate flow
    // If not called otherwise, the flow algorithm uses the interior properties
    // - edge_capacity, edge_reverse (read access),
    // - edge_residual_capacity (read and write access).
    
    if(totalSoldiers < totalReq) {
        cout<<"no"<<endl;
        return;
    }
    long flow = push_relabel_max_flow(G, src, sink);
    int expectedFlow = totalReq;
    rep(i,l) {
        expectedFlow += inMinCapSum[i];
    }
    //cout<<flow<<endl;
    if(flow==expectedFlow) cout<<"yes"<<endl;
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
            kindom();
            //cout<<endl;
        }
	return 0;
}
