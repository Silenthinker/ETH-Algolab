// - MinCostMaxFlow with negative edge costs using cycle_canceling
// - MinCostMaxFlow with negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 carsharing.cpp -o carsharing; ./carsharing

// Includes
// ========
// STL includes
#include <iostream>
#include <set>
#include <map>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
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
typedef graph_traits<Graph>::edge_iterator EdgeIt; 

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

// custom request struct
struct Request {
    int s, t, d, a, p;
};

void carsharing() {
    int N, S; cin >> N >> S; // # of requests and rental stations
    int MINT = 0;
    int MAXT = 100000;
    int MAXP = 100;
    int INF = 1000;
    vector<int> cars(S);
    vector<Request> requests(N); // si, ti, di, ai, pi
    vector<set<int> > TIME(S);
    vector<map<int, int> > M(S);
    vector<int> indexStations(S+1);
    rep(i, S) cin >> cars[i];
    rep(i, S) {
        TIME[i].insert(MINT);
        TIME[i].insert(MAXT);
    }
    rep(i, N) {
        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        s--; t--;
        requests[i] = {s, t, d, a, p};
        TIME[s].insert(d);
        TIME[t].insert(a);
    }
    rep(i, S) {
        int j = 0;
        for(auto &t: TIME[i]) {
            M[i][t] = j++;
        }
        indexStations[i+1] = indexStations[i] + M[i].size();
    }
    
    
    // Create Graph and Maps
    const int T = indexStations.back();
    const Vertex v_source = T, v_target = T + 1;
    const int nVertices = T + 2;
    Graph G(nVertices);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    rep(i, S) {
        eaG.addEdge(v_source, indexStations[i], cars[i], 0);
        eaG.addEdge(indexStations[i+1] - 1, v_target, INF, 0);
        int start = -1, lt = 0;
        for(auto &t: TIME[i]) {
            if (start != -1) {
                eaG.addEdge(indexStations[i]+start, indexStations[i]+start+1, INF, (t - lt) * MAXP);
            }
            start++; lt = t;
        }
    }
    
    rep(i, N) {
        eaG.addEdge(indexStations[requests[i].s]+M[requests[i].s][requests[i].d], indexStations[requests[i].t]+M[requests[i].t][requests[i].a], 1, -1*requests[i].p + MAXP * (requests[i].a - requests[i].d));
        //cout << requests[i].s << ": " << requests[i].d << "->" << requests[i].t << ": " << requests[i].a << "with " << requests[i].p << endl;
        //cout << "M[s][t]" << M[requests[i].s][requests[i].d] << ", " << M[requests[i].t][requests[i].a] << endl;
    }
    
    successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = find_flow_cost(G);
    int flow = 0;
    OutEdgeIt e, eend;
    
    for(tie(e, eend) = out_edges(vertex(v_source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    /*
    EdgeIt edbeg, edend;
    for(tie(edbeg, edend) = edges(G); edbeg != edend; ++edbeg) {
        if (capacitymap[*edbeg] != 0) {
            cout << source(*edbeg, G) << "->" << target(*edbeg, G) << ": " << capacitymap[*edbeg] << ", " << weightmap[*edbeg] << endl;
        }
    }
    */
    cout << (MAXT - MINT) * MAXP * flow - cost << endl;
    //cout << flow << endl;
    
}


int main() {
    int T; // # of test cases
    cin >> T;
    rep(_, T) {
        carsharing();
    }
    return 0;
}

