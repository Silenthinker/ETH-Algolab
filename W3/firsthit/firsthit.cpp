/* Arrange segment in such an order that we test
 * if hit takes place from nearest to farthest to 
 * the source of ray
 * Ex. if ray goes from left to right, arrange segment by
 * its leftmost endpoint.
 * Since there are two cases (ray's direction to left or to right), 
 * only consider the right case by transforming left to right
 * */

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <queue>

#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

struct MyCmpStruct
{
    bool operator()(const S &lhs, const S &rhs){
        P lhs_src = lhs.source();
        P rhs_src = rhs.source();
        return (lhs_src.x() > rhs_src.x());// segment with source with smaller x comes first; we want MIN pq!
    }
};


void firsthit0(int n) 
{
    long long x,y,a,b,r,s,t,u;
    std::cin>>x>>y>>a>>b; // coordinate of ray
    S ob;
    bool direction;
    if(a>=x)
    {
        direction = true;
    }
    else
    {
        // do the transformation
        x = -1*x;
        a = -1*a;
        direction = false;
        
    }
    R ray = R(P(x,y),P(a,b)); // ray
    bool everHit = false;
    std::priority_queue<S,std::vector<S>,MyCmpStruct> pq;
    P firstHitPt = P(LLONG_MAX,LLONG_MAX);
    rep(_,n)
    {
        std::cin>>r>>s>>t>>u; // obstacle
        if(!direction)
        {
            r = -1*r;
            t = -1*t;
        }
        if(r<=t) // ensure source is left to target
        {
            ob  = S(P(r,s),P(t,u));
        }
        else
        {
            ob = S(P(t,u),P(r,s));
        }
        pq.push(ob);
    }
    rep(_,n)
    {
        ob = pq.top();
        pq.pop();
        P ob_src = ob.source();
        if(ob_src.x()<firstHitPt.x())
        {
            // case of collinearity
            if(CGAL::collinear(ray.source(),ray.point(1),ob.source()) && CGAL::collinear(ray.source(),ray.point(1),ob.target()))
            {
                if(ob_src.x() < firstHitPt.x())
                {
                    firstHitPt = ob_src;
                } 
            }
            if(CGAL::do_intersect(ray,ob))
            {
                everHit = true;
                auto hitPt = CGAL::intersection(ray,ob);
                if (const P* hitPtp = boost::get<P>(&*hitPt))
                {
                    if(hitPtp->x() < firstHitPt.x())
                    {
                        firstHitPt = *hitPtp;
                    }
                }
                
            }
        }
        else
        {
            break;
        }
    }
    if(everHit)
    {
        if(!direction)
        {
            std::cout<<std::fixed<<std::setprecision(0)<<floor_to_double(-1*firstHitPt.x())<<" "<<floor_to_double(firstHitPt.y());
        }
        else
        {
            std::cout<<std::fixed<<std::setprecision(0)<<floor_to_double(firstHitPt.x())<<" "<<floor_to_double(firstHitPt.y());
        }
        
    }
    else
    {
        std::cout<<"no";
    }
    return;
    
}


int main(void)
{
    std::ios_base::sync_with_stdio(false);
    int n; // # of obstacle segments
    while(true)
    {
        std::cin>>n;
        if(n!=0)
        {
            firsthit0(n);
            std::cout<<std::endl;
        }
        else
        {
            break;
        }
        
    }
    return 0;
}