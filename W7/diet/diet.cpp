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

void diet(int n, int m) {
    vector< pair< int, int > > nutrients(n, pair< int, int >()); // pair(min, max)
    vector< vector< int > > food(m, vector< int >(n, 0)); // food[i][j]: ith food contains food[i][j] amount jth food;
    vector< int > price(m, 0);
    rep(i, n) {
        int minAmout, maxAmout;
        cin >> minAmout >> maxAmout;
        nutrients[i] = make_pair(minAmout, maxAmout);
    }
    rep(i, m) {
        int p, amout;
        cin >> p;
        price[i] = p;
        rep(j, n) {
            cin >> amout;
            food[i][j] = amout;
        }
    }
    Program lp (CGAL::SMALLER, true, 0, false, 0); // x_i >= 0
    rep(j, n) {
        rep(i, m) {
            lp.set_a(i, j, food[i][j]); lp.set_b(j, nutrients[j].second);
            lp.set_a(i, j + n, -1*food[i][j]); lp.set_b(j + n, -1*nutrients[j].first);
        }
    }
    rep(i, m) {
        lp.set_c(i, price[i]);
    }
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    if (s.is_infeasible()) {
        cout << "No such diet.";
    } else {
        cout << fixed << setprecision(0) << floor(CGAL::to_double(s.objective_value()));
    }   
}

int main() {
    int n, m; // # of nutrients and of food
    while(true) {
        cin >> n >> m;
        if(n == 0 && m == 0) break;
        diet(n, m);
        cout << endl;
    }
    return 0;
}
