/* Algorithm
 * BFS to find a way out
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>
#include <map>

using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Triangulation_vertex_base_2<IK> Vertex_t;
typedef CGAL::Triangulation_face_base_with_info_2<bool, IK> Face_t;
typedef CGAL::Triangulation_data_structure_2<Vertex_t, Face_t> Triangulation_t;
typedef CGAL::Delaunay_triangulation_2<IK, Triangulation_t>  Delaunay_t;
typedef Delaunay_t::All_faces_iterator  Face_iterator;
typedef Delaunay_t::Edge_iterator  Edge_iterator;
typedef Delaunay_t::Finite_vertices_iterator Vertex_iterator;
typedef Delaunay_t::Face_handle Face_handle;
typedef IK::Point_2 Point;

#define forloop(i,lo,hi) for(int i = (lo); i <= (hi); ++i)
#define rep(i,N) forloop(i,0,(int)N-1)

void h1n1(int n){
    vector<Point> pts;
    pts.reserve(n);
    rep(i,n){ // infected people
        Point p;
        cin >> p;
        pts.push_back(p);
    }
    // construct triangulation
    Delaunay_t T;
    T.insert(pts.begin(), pts.end());
    
    int m; cin >> m;
    rep(i,m){ // users
        int x,y;
        long long d;
        cin >> x >> y >> d;
        Point p(x,y);
        // check initial position
        if( CGAL::squared_distance(T.nearest_vertex(p)->point(),p)<d ) // nearest vertex distance
            {cout<< "n"; continue;}
        Face_handle fh0 = T.locate(p); // the face that p is in
        // caching will not work because the ds are different! 
        // --> need to map each fh to the max d that allows to reach from fh to inf face
        //~ if( res_cache.count(fh0)==1 ) { // if result cached before
            //~ {cout << (res_cache[fh0]?'y':'n'); continue;}
        // otherwise do a BFS to try to get to an infinite face
        
        for (Face_iterator f = T.all_faces_begin(); f != T.all_faces_end(); ++f) {
            f -> info() = false;
        }
        
        bool success=false;
        queue<Face_handle> q; 
        q.push(fh0); fh0 -> info() = true;
        while(!q.empty()){
            Face_handle fh = q.front(); q.pop();
            if(T.is_infinite(fh))
                {success=true;break;}
            rep(i,3){
                Face_handle nextfh = fh->neighbor(i);
                if( T.segment(fh,i).squared_length()>=4*d && (!nextfh -> info()) )
                    { 	if( T.is_infinite(nextfh) ) {success=true; break;}
                        q.push(nextfh); nextfh -> info() = true;
                    }
            } // go over adj faces
        } // bfs
        cout << (success?"y":"n");
        
    }
    cout << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    while(1){
        cin >> n;
        if(n==0) return 0;
        h1n1(n);
    }
    return 0;
}