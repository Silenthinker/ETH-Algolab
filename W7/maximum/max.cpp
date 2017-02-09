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

using namespace std;

void max(int p, int a, int b) {
    const int X = 0;
    const int Y = 1;
    const int Z = 2; // Z = z^2
    if(p==1) {
        // by default, we have a nonnegative LP with Ax <= b
        Program qp (CGAL::SMALLER, true, 0, false, 0); // x, y >= 0
        // pb max
        qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, 4); // x + y <= 4
        qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_b(1, a*b); // 4x + 2y <= a*b
        qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, 1); // -x + y <= 1
        qp.set_d(X, X, 2*a); // max by-ax^2 <=> min -by+ax*2
        qp.set_c(Y, -b);
        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
        assert (s.solves_quadratic_program(qp));
        // output
        if (s.status() == CGAL::QP_INFEASIBLE) {
            cout << "no";
        } else if (s.status() == CGAL::QP_OPTIMAL) {
            cout << fixed << setprecision(0) << floor(-1*CGAL::to_double(s.objective_value()));
        }   
        else cout << "unbounded";
    }
    else {
        // by default, we have a nonnegative LP with Ax >= b
        Program qp (CGAL::LARGER, true, 0, false, 0); // -x, -y, z^2 >= 0
        // pb min
        qp.set_a(X, 0, -1); qp.set_a(Y, 0, -1); qp.set_a(Z, 0, 0); qp.set_b(0, -4); // x + y >= 4
        qp.set_a(X, 1, -4); qp.set_a(Y, 1, -2); qp.set_a(Z, 1, 1); qp.set_b(1, -a*b); // 4x + 2y + z^4 >= -a*b
        qp.set_a(X, 2, 1); qp.set_a(Y, 2, -1); qp.set_a(Z, 2, 0); qp.set_b(2, -1); // -x + y >= -1
        qp.set_d(X, X, 2*a); // min by+ax*2+z^4
        qp.set_d(Z, Z, 2);
        qp.set_c(Y, -b);
        Solution s = CGAL::solve_quadratic_program(qp, ET());
        assert (s.solves_quadratic_program(qp));
        if (s.status() == CGAL::QP_INFEASIBLE) {
            cout << "no";
        } else if (s.status() == CGAL::QP_OPTIMAL) {
           cout << fixed << setprecision(0) << ceil(CGAL::to_double(s.objective_value()));
        }   
        else cout << "unbounded";
    }

}

int main() {
    int p, a, b; // type of pb, val of param
    while(true) {
        cin >> p;
        if(p == 0) break;
        cin >> a >> b;
        max(p, a, b);
        cout << endl;
    }
    return 0;
}
