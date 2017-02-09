//STL
#include <iostream>
#include <vector>
#include <utility>
//CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle vhandle_t;
typedef Triangulation::Vertex_circulator vcirculator_t;

using namespace std;

// User defined
#define forloop(i, l, h) for (int i = (l); i < (h); i ++)
#define rep(i, N) forloop(i, 0, N)

bool rebel(int k, int n, K::FT &dist, vector<K::Point_2> &planets) {
    
    vector<K::Point_2> rebel_pool;
    forloop(i, k, n) rebel_pool.push_back(planets[i]);
    // construct triangulation
    Triangulation t;
    t.insert(rebel_pool.begin(), rebel_pool.end());
    
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) v->info() = 0; // by default, if no component assigned, set to 0
    
    int component = 0;
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
        vector<vhandle_t> stack;
        stack.push_back(v);
        if (v->info() == 0) v->info() = ++component;
        else continue;
        do {
            vhandle_t u = stack.back(); stack.pop_back();
            vcirculator_t c = t.incident_vertices(u);
            do if (!t.is_infinite(c)) {
                if (c->info() == 0 && CGAL::squared_distance(c->point(), u->point()) <= dist) {
                    c->info() = u->info();
                    stack.push_back(c);
                }
            } while (++c != t.incident_vertices(u));
        } while (!stack.empty());
    }
    vector<int> ncomponents(component, 0);
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
        ncomponents[v->info()-1]++;
    }
    int maxncomponent = 0;
    rep(i, component) {
        //cout << "ncomponents" << ncomponents[i] << endl;
        maxncomponent = max(maxncomponent, ncomponents[i]);
    }
    if (maxncomponent >= k) return true;
    else return false;
    
    //return true;
}

void sith() {
    int n; K::FT r; cin >> n >> r; // # of planets; operation range
    K::FT dist = r*r;
    vector<K::Point_2> planets;
    rep(i, n) {
        K::Point_2 p; cin >> p;
        planets.push_back(p);
    }
    int l = 1, h = n / 2 + 1; // inclusive
    while (l != h) {
        int m = l + (h - l) / 2;
        if (rebel(m, n, dist, planets)) l = m + 1;
        else h = m;
    }
    // optimal k is l - 1
    cout << l - 1 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t; // # of test cases
    rep(_, t) sith();
    return 0;
}
