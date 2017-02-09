
// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

// STL
#include <vector>
#include <iostream>
#include <utility> // pair

#define forloop(i, l, h) for(int i=(l); i<(h); i++)
#define rep(i, N) forloop(i, 0, N)

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;

using namespace std;

double ceil_to_double(EK::FT const & x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x)
    a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void clues() {
    int n, m ,r; cin >> n >> m >> r; // # of stations, of clues and range
    int x, y;
    vector< pair<int, int> > stations(n), holmes(m), watson(m);
    rep(i, n) {
        cin >> x >> y;
        stations[i] = make_pair(x, y);
    }
    rep(i, m) {
        cin >> x >> y; holmes[i] = make_pair(x, y);
        cin >> x >> y; watson[i] = make_pair(x, y);
    }
}

int main() {
    int t; cin >> t; // # of test cases
    rep(_, t) clues();
    return 0;
}