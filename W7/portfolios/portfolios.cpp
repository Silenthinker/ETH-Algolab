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

void portfolios(int n, int m) {
    vector< int > c(n, 0), r(n, 0); // cost and return
    vector< vector< int > > cov(n, vector< int >(n, 0)); // covariance
    vector< vector< int > > investors(m, vector< int >(3, 0)); // investors[i]: (max_cost, min expected return, and max variances)
    int cost, ret, v;
    rep(i, n) {
        
        cin >> cost >> ret;
        c[i] = cost;
        r[i] = ret;
    }
    rep(i, n) {
        rep(j, n) {
            cin >> v;
            cov[i][j] = v;
        }
    }
    rep(i, m) {
        rep(j, 3) {
            cin >> v;
            investors[i][j] = v;
        }
    }
    Program qp(CGAL::SMALLER, true, 0, false, 0); // alpha_i >= 0
    rep(k, m) {
        rep(i, n) {
            qp.set_a(i, 0, -1*r[i]); qp.set_b(0, -1*investors[k][1]);
            qp.set_a(i, 1, c[i]); qp.set_b(1, investors[k][0]);
        }
        rep(i, n) {
            rep(j, n) {
                qp.set_d(i, j, 2*cov[i][j]);
            }
        }
        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
        assert (s.solves_quadratic_program(qp));
        // output
        if (s.status() == CGAL::QP_OPTIMAL) {
            if (CGAL::to_double(s.objective_value()) <= investors[k][2]) cout << "Yes.";
            else cout << "No.";
        }
        else cout << "No.";
        
        cout<<endl;
    }
}

int main() {
    int n, m; // # of total assets and of people
    while(true) {
        cin >> n >> m;
        if(n == 0 && m == 0) break;
        portfolios(n, m);
    }
    return 0;
}
