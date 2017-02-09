/* Algorithm
 * Construct delaunay triangulation to
 * find nearest vertex of a given point
 */

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

double ceil_to_double(EK::FT const & x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x)
    a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void test_case(int n) {
    std::vector<IK::Point_2> bistro(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> bistro[i];
    }
    
    // Construct delaunay triangulation
    typedef CGAL::Delaunay_triangulation_2<IK> delaunay_t;
    delaunay_t trg;
    trg.insert(bistro.begin(), bistro.end());
    
    // Find nearest vertices
    int nNewLocations;
    std::cin >> nNewLocations;
    Face_iterator f = trg.finite_faces_begin(); // initialize the searching location
    rep(_, nNewLocations) {
        IK::Point_2 p;
        std::cin >> p;
        Triangulation::Vertex_handle v = trg.nearest_vertex(p, f);
        std::cout << std::fixed << std::setprecision(0) << CGAL::squared_distance(v->point(), p) << std::endl;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n;
    while (std::cin >> n && n != 0) test_case(n);
}