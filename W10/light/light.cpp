/* Algorithm
 * Order of lamps does affect result!
 * Some cases exist where no people remain at last.
 * However, the problem still requires finding out those who survive longest!
 * Since lighs are added gradually, the number of people at the stage decreases monotocally, which boils down to a binary search problem: find the point where people all lose if one more light switches on.
 */

// STL
#include <vector>
#include <iostream>
#include <algorithm>

//CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// User defined
#define forloop(i, l, h) for(int i = (l); i < (h); i++)
#define rep(i, N) forloop(i, 0, N)

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef std::vector<K::Point_2>::iterator point_iterator;

using namespace std;

vector<int> findSurvivors(point_iterator from, point_iterator to,  vector<K::Point_2>& lamps, vector<K::Point_2>& participants, vector<long>& radius, long h) {
    Triangulation t;
    t.insert(from, to);
    Face_iterator f = t.finite_faces_begin();
    vector<int> survivors;
    int m = participants.size();
    rep(i, m) {
        Triangulation::Vertex_handle v = t.nearest_vertex(participants[i], f);
        //long dist = (h+radius[i])*(h+radius[i]);
        long dist = h+radius[i];
        /*cout << "nearest_vertex = " << v->point() << " " << "participant = " << participants[i] << " with radius = " << radius[i]<< endl;
        cout << "dist = " << dist << " " << "pairD = " << CGAL::squared_distance(v->point(), participants[i]) << endl;;
        */
        if (sqrt(CGAL::squared_distance(v->point(), participants[i])) >= dist) {
            survivors.push_back(i);
        }
    }
    return survivors;
}

void light() {
    int m, n; // # of participants and lamps
    cin >> m >> n;
    vector<K::Point_2> participants, lamps;
    participants.reserve(m); lamps.reserve(n);
    vector<long> radius; radius.reserve(m);
    rep(_, m) {
        K::Point_2 p; long r;
        cin >> p >> r;
        participants.push_back(p);
        radius.push_back(r);
    }
    long h; cin >> h; // height
    rep(_, n) {
        K::Point_2 p;
        cin >> p;
        lamps.push_back(p);
    }
    vector<int> survivors = findSurvivors(lamps.begin(), lamps.end(), lamps, participants, radius, h);
    //cout << survivors.size() << endl;
    if (survivors.empty()) {
        int li = 0, hi = n - 1, mi; // from hi to n-1
        while (li != hi) {
            mi = (li + hi) / 2;
            survivors = findSurvivors(lamps.begin(), lamps.begin() + 1 + mi, lamps, participants, radius, h);
            if (survivors.empty()) {
                hi = mi;
            } 
            else li = mi + 1;
        }
        survivors = findSurvivors(lamps.begin(), lamps.begin() + hi, lamps, participants, radius, h);
    }
    //cout << survivors.size() << endl;
    sort(survivors.begin(), survivors.end());
    rep(i, survivors.size()) {
        cout << survivors[i] << " ";
    }
    cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t; // # of test cases
    rep(_, t) light();
    return 0;
}