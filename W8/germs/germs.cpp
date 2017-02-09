/* Algorithm
 * Construct delaunay triangulation
 * Compute d_min = min(d(i,j), distance to nearest border)
 * case 1: d_min = min(d(i, j)), 2(t^2+1/2)>=d_min
 * case 2: d_min = distance to nearest border, t^2+1/2>=d_min
 * then sort time
 */

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Triangulation_vertex_base_with_info_2<IK::FT, IK> Vertex_t;
typedef CGAL::Triangulation_face_base_2<IK> Face_t;
typedef CGAL::Triangulation_data_structure_2<Vertex_t, Face_t> Triangulation_t;
typedef CGAL::Delaunay_triangulation_2<IK, Triangulation_t>  Delaunay_t;
typedef Delaunay_t::Finite_faces_iterator  Face_iterator;
typedef Delaunay_t::Edge_iterator  Edge_iterator;
typedef Delaunay_t::Finite_vertices_iterator Vertex_iterator;

#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

double ceil_to_double(EK::FT const & x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x)
    a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

EK::FT S_to_hrs(EK::FT const & S) {
    return CGAL::sqrt((CGAL::sqrt(S) - 1) / 2);
}


void test_case(int n) {
    std::vector<IK::Point_2> germs(n);
    IK::FT l, b, r, t;
    std::cin >> l >> b >> r >> t;
    
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> germs[i];
    }
    
    // Construct delaunay triangulation
    Delaunay_t trg;
    trg.insert(germs.begin(), germs.end());
    
    // For all vertices, find nearest distance to border
    for (Vertex_iterator v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); v++){
        IK::Point_2 const & g = v->point();
        IK::FT & info_g = v->info();
        IK::FT min_horizontal = std::min(g.x() - l, r - g.x());
        IK::FT min_vertical = std::min(g.y() - b, t - g.y());
        IK::FT min_overall = std::min(min_horizontal, min_vertical);
        info_g = 4 * min_overall * min_overall;
    }
    
    // For all vertices, find nearest pairwise distance
    for (Edge_iterator e = trg.finite_edges_begin(); e != trg.finite_edges_end(); e++) {
        Delaunay_t::Vertex_handle vi = e->first->vertex(trg.cw(e->second));
        IK::Point_2 const & gi = vi->point();
        IK::FT & info_gi = vi->info();
        Delaunay_t::Vertex_handle vj = e->first->vertex(trg.ccw(e->second));
        IK::Point_2 const & gj = vj->point();
        IK::FT & info_gj = vj->info();
        IK::FT squared_distance = CGAL::squared_distance(gi, gj);
        info_gi = std::min(info_gi, squared_distance);
        info_gj = std::min(info_gj, squared_distance);
    }
    
    // Final results
    std::vector<IK::FT> min_S;
    min_S.reserve(n);
    for (Vertex_iterator v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); ++v) {
        min_S.push_back(v->info());
    }
    std::sort(min_S.begin(), min_S.end());
    EK::FT first = S_to_hrs(EK::FT(min_S[0]));
    EK::FT median = S_to_hrs(EK::FT(min_S[n/2]));
    EK::FT last = S_to_hrs(EK::FT(min_S[n-1]));
    std::cout << ceil_to_double(first) << " " << ceil_to_double(median) << " " << ceil_to_double(last) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n;
    while (std::cin >> n && n != 0) test_case(n);
}