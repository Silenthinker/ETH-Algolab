/*
 * cgal_create_cmake_script
 * cmake .
 * If set C++11
 * set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
 */

/*
 * min -r
 * s.t. a^T_ix_c + r||a_i||_i <= b_i
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
#include <CGAL/Gmpz.h>


// choose exact floating-point type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// User-defined
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

using namespace std;

void inball(int n, int d) {
    vector< vector < int > > inequalities(n, vector< int >(d, 0)); // inequalities[i][j] = (a_i)_j
    vector< int > norm(n, 0);
    vector< int > rhs(n, 0); // b
    int a, b;
    rep(i, n) {
        rep(j, d) {
            cin >> a;
            inequalities[i][j] = a;
            norm[i] += a*a;
        }
        norm[i] = sqrt(norm[i]);
        cin >> b;
        rhs[i] = b;
    }
    Program lp (CGAL::SMALLER, false, 0, false, 0); //by default no upper and lower constraint on x
    rep(i, n) {
        rep(j, d) {
            lp.set_a(j, i, inequalities[i][j]); lp.set_a(d, i, norm[i]); lp.set_b(i, rhs[i]);
        }
    }
    lp.set_c(d, -1);
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    // ouput
    
    if (s.status() == CGAL::QP_INFEASIBLE) cout << "none";
    else if (s.status() == CGAL::QP_OPTIMAL) {
        double r = -1*CGAL::to_double(s.objective_value());
        if (r >= 0) cout << floor(r);
        else cout << "none";
    }
    else cout << "inf";
}

int main() {
    int n, d; // # of inequalities and dimension
    while(true) {
        cin >> n;
        if(n == 0) break;
        cin >> d;
        inball(n, d);
        cout << endl;
    }
    return 0;
}
