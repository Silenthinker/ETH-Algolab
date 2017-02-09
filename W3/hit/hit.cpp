#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <cmath>

#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;


void hit(int n)
{
    long long x,y,a,b,r,s,t,u;
    std::cin>>x>>y>>a>>b; // coordinate of ray
    R ray = R(P(x,y),P(a,b)); // ray
    bool isHit = false;
    rep(_,n)
    {
        std::cin>>r>>s>>t>>u; // obstacle
        S ob = S(P(r,s),P(t,u));
        // std::cout<<x<<y<<a<<b<<std::endl;
        // std::cout<<r<<s<<t<<u<<std::endl;
        if(!isHit) // if already hit, do not check
        {
            if(CGAL::do_intersect(ray,ob))
            {
                std::cout<<"yes";
                isHit = true;
            }
        }
    }
    if(!isHit)
    {
        std::cout<<"no";
    }
    return;
}


int main(void)
{
    int n; // # of obstacle segments
    while(true)
    {
        std::cin>>n;
        if(n!=0)
        {
            hit(n);
            std::cout<<std::endl;
        }
        else
        {
            break;
        }
        
    }
    return 0;
}