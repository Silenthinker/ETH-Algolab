/* Similar to search snippet: sliding window
 * Use orientation to determine whether a point is included
 * inside a triangle
 * 
 */

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <utility> // pair
#include <queue>
#include <climits>

#define forloop(i, l, h) for (int i=(l); i< (h); i++)
#define rep(i, N) forloop(i, 0, N)

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef IK::Point_2 Point;

using namespace std;

struct MyComparison {
    bool operator() (const pair<int, int>& lhs, const pair<int, int>& rhs) const {
        return (lhs.second > rhs.second);
    }
};


void orderPointDirectionByCounterClockwise(vector<Point>& triangle) {
    if (CGAL::right_turn(triangle[0], triangle[1], triangle[2])) {
        std::swap(triangle[0], triangle[1]);
    }
    rep(i, 2) {
        if (CGAL::right_turn(triangle[2*i+2], triangle[2*i+3], triangle[0])) {
            std::swap(triangle[2*i+2], triangle[2*i+3]);
        }
    }
}

bool isPointInsideTriangle(Point& pt, vector<Point>& triangle) {
    rep(i, 3) {
        if (CGAL::right_turn(triangle[2*i], triangle[2*i+1], pt)) {
            return false;
        }
    }
    return true;
}

bool isSegmentInsideTriangle (Point& p, Point& q, vector<Point>& triangle) {
    return isPointInsideTriangle(p, triangle) && isPointInsideTriangle(q, triangle);
}

void hiking() {
    std::size_t m, n; 
    int x, y; // m-1 for # of legs, n for # of map parts
    std::cin >> m >> n;
    std::vector<Point> pts(m);
    std::vector< std::vector<Point> > triangles(n, std::vector<Point>(6));
    std::vector< vector<int> > posting(m-1, vector<int>());
    vector<int> cover(m-1, 0);
    rep(i, m) {
        std::cin >> pts[i];
    }
    rep(i, n) {
        rep(j, 6) {
            std::cin >> triangles[i][j];
        }
        orderPointDirectionByCounterClockwise(triangles[i]);
    } 
    
    rep(i, m-1) {
        rep(j, n) {
            if (isSegmentInsideTriangle(pts[i], pts[i+1], triangles[j])) {
                posting[i].push_back(j);
                cover[i]++;
            }
        }
    }
    
    //////////////
    vector<int> indices(m-1);
    priority_queue< pair<int, int>, vector< pair<int, int> >, MyComparison > pq; // <index, pos>
    pair<int, int> minPair;
    int idxMinPos, minPos, maxPos = -1;
    rep(i, m-1) {
        pq.push(make_pair(i, posting[i][0]));
        maxPos = max(maxPos, posting[i][0]);
    }


    //cout << maxPos << endl;;
    int len = INT_MAX;
    while (1) {
        minPair = pq.top();
        idxMinPos = minPair.first; minPos = minPair.second;
        len = min(len, maxPos - minPos + 1);
        //cout << len << " ";
        if (cover[idxMinPos] <= indices[idxMinPos]) break;
        indices[idxMinPos]++;
        if (posting[idxMinPos][indices[idxMinPos]] > maxPos) {
            maxPos = posting[idxMinPos][indices[idxMinPos]];
        }
        pq.pop();
        pq.push(make_pair(idxMinPos, posting[idxMinPos][indices[idxMinPos]]));
    }
    cout << len << "\n";
    /*
    rep(i, m-1) {
        rep(j, posting[i].size()) {
            cout << posting[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    */
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int c;
    std::cin >> c;
    rep(_, c) {
        hiking();
    }
    return 0;
}