/* Moving books
 * 
 * Greedy algorithm: within a given round, we process the people one by one, making each person
 * move the heaviest box that he is able to lift and that has not already been moved, provided
 * such a box exists.
 */

#include <iostream>
#include <cmath>
#include <climits>
#include <queue> // std::priority_queue
#include <vector>
#include <map>
#include <algorithm>
#include <functional>   // std::greater

using namespace std;
#define forloop(i,lo,hi) for(int i = (lo); i < (hi); i++)
#define rep(i,N) forloop(i,0,N)

void books()
{
    int n,m; // number of friends and boxes, respectively
    int s,w; // strength, weight
    int minTime = 0;
    priority_queue<int, vector<int>, greater<int> > pq;
    map< int,pair<int,queue<int> > > mymap; // < strength,< num,pqboxes<int> > >
    cin>>n; cin>>m;
    rep(_,n)
    {
        cin>>s;
        if(mymap.find(s)==mymap.end())
        {
            // if no pair, add it
            queue<int> q;
            mymap[s] = make_pair(1,q);
        }
        else
        {
             mymap[s].first++;
        }
    }
    rep(_,m)
    {
        cin>>w;
        pq.push(w);
    }
    for(map<int,pair<int,queue<int> > >::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    {
        while(!pq.empty())
        {
            if(it->first >= pq.top())
            {
                // capable to carry, thus put to one's list
                (it->second).second.push(pq.top());
                pq.pop();
            }
            else
                break; // else see next friend
        }
    }
    // ideally, pq should be empty; otherwise impossible
    if(!pq.empty())
    {
        cout<<"impossible\n";
        return;
    }
    
    // let's start
    int nBoxes = m;
    while(nBoxes>0)
    {
        for(map<int,pair<int,queue<int> > >::reverse_iterator it=mymap.rbegin(); it!=mymap.rend(); ++it)
        {
            // in reverse order: from one with most strength
            int nFriendAvaliable = (it->second).first;
            while((!(it->second).second.empty() && nFriendAvaliable>0))
            {
                (it->second).second.pop();
                nFriendAvaliable--;
                nBoxes--;
            }
            if(nFriendAvaliable>0)
            {
                // make the friend to help the one with less strength
                map<int,pair<int,queue<int> > >::reverse_iterator next = it;
                next++;
                if(next!=mymap.rend())
                {
                    (next->second).first += nFriendAvaliable;
                    (it->second).first -= nFriendAvaliable;
                }
            }
        }
        minTime += 3;
    }
    // minus the extra 1 min
    minTime -= 1;
    
    cout<<minTime<<"\n";
}


int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t; // t cases
    rep(_,t)
    {
        books();
    }
    return 0;
}
