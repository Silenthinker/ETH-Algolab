#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <queue>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
typedef  CGAL::Min_circle_2_traits_2<K> Traits;
typedef  CGAL::Min_circle_2<Traits> Min_circle;

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a >= x+1) a -= 1;
    while (a < x) a += 1;
    return a;
}

void antenna(int n)
{
    long int x,y;
    P p[n];
    rep(i,n)
    {
        cin>>x>>y;
        p[i] = P(x,y);
    }
    Min_circle mc(p, p+n, true); // faster using randomization
    Traits::Circle c = mc.circle();
    cout<<fixed<<setprecision(0)<<ceil_to_double(sqrt(c.squared_radius()));
}

int main(void)
{
    ios_base::sync_with_stdio(false);
    int n; // # of citizens
    while(true)
    {
        cin>>n;
        if(n==0) break;
        antenna(n);
        cout<<endl;
    }
    return 0;
}