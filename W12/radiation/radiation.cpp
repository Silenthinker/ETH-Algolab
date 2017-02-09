/*
 * cgal_create_cmake_script
 * cmake .
 * If set C++11
 * set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
 */


// std
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
// CGAL
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>


// choose exact floating-point type
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// User-defined
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

using namespace std;

struct Position {
    int x, y, z;
};

// push back polynomial from d1 to d2 exclusive into var
void poly(int d1, int d2, vector<Position> &pos, vector<vector<double> > &var) {
    for(size_t n = 0; n < pos.size(); n++) {
        forloop(i, 0, d2) {
            forloop(j, 0, d2-i) {
                forloop(k, 0, d2-i-j) {
                    if (i+j+k >= d1) {
                        if (i+j+k != 0) {
                            var[n].push_back(pow(pos[n].x, i)*pow(pos[n].y, j)*pow(pos[n].z, k));
                        }
                        else var[n].push_back(1);
                    }
                }
            }
        }
    }
    return;
}

bool solve(vector<vector<double> > &var, int h, int d) {
    Program lp (CGAL::SMALLER, false, 0, false, 0); //by default no upper and lower constraint on x
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    int nVar = 0;
    forloop(i, 0, d) {
            forloop(j, 0, d-i) {
                forloop(k, 0, d-i-j) {
                    ++nVar;
                }
            }
    }
    //const int delta = var[0].size();
    const int delta = nVar;
    for(size_t i = 0; i < var.size(); i++) {
        if (i < h) {
            for(size_t j = 0; j < nVar; j++) {
                lp.set_a(j, i, -1*var[i][j]);
                lp.set_b(i, 0);
            }
            lp.set_a(delta, i, -1);
        } else {
            for(size_t j = 0; j < nVar; j++) {
                lp.set_a(j, i, var[i][j]);
                lp.set_b(i, 0);
            }
        }
    }
    lp.set_l(delta, true, -1);
    lp.set_c(delta, 1);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    assert(s.solves_linear_program(lp));
    // ouput
    if (s.status() == CGAL::QP_OPTIMAL && (s.objective_value() < 0)) return true;
    else return false;
}

void radiation() {
    int h, t; cin >> h >> t; // # of healthy cell and tumor cell
    int total = h + t;
    vector<Position> pos; pos.reserve(total);
    vector<vector<double> > var(total, vector<double>());
    rep(_, total) {
        int x, y, z; cin >> x >> y >> z;
        pos.push_back({x, y, z});
    }
//     rep(i, pos.size()) {
//         cout << pos[i].x << " " << pos[i].y << " " << pos[i].z << endl;
//     }

    int maxD = 30;
    int lb = maxD+1;
    /*
    forloop(i, 0, maxD+1) {
        poly(i, i+1, pos, var);
        if (solve(var, h, i+1)) {
            lb = i;
            break;
        }
    }
    if (lb <= maxD) cout << lb;
    else cout << "Impossible!";
    cout << "\n";
    */
    
    int low = 0, high, k = 1;
    high = pow(2, k);
    bool success = false;
    forloop(i, 0, maxD+1) {
        poly(i, i+1, pos, var);
        if (i + 1 == high) {
            if (solve(var, h, high)) {
                success = true;
                break;
            } else {
                low = high;
                high = pow(2, ++k);
                if (high > maxD+1) high = maxD+1;
            }
        }
    }
    /*
    for (int k = 1; k < 6; k++) {
        if (k == 1) {
            low = 0; high = pow(2, k);
        }
        else if (k == 5) {
            low = pow(2, k - 1); high = 31;
        }
        else {
            low = pow(2, k - 1); high = pow(2, k);
        }
        //cout << low << "->" << high << endl; 
        if (solve(var, h, high)) {
            success = true;
            break;
        }
    }
    */
    // BS
    
    while (high - low > 1) {
        int mid = (high + low) / 2;
        if (solve(var, h, mid)) {
            high = mid;
        } else {
            low = mid;
        }
    }
    
    if (success) cout << low;
    else cout << "Impossible!";
    cout << endl;
    
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int N; cin >> N; // # of testcases
    rep(_, N) radiation();
    return 0;
}
