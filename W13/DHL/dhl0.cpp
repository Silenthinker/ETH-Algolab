#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void computePartialSum(int S[],int n)
{
  int sum =0;
  for(int i=1;i<n;i++)
    {
      int no;
      cin>>no;
      sum+= (no - 1);
      S[i]= sum;
    }
  S[0]=0;
}
int main()
{
    std::ios_base::sync_with_stdio(false);
  int t;
  cin>>t;
  while (t--)
    {
      int n;
      cin>>n;

      int Sa[n+1], Sb[n+1];
      computePartialSum(Sa,n+1);
      computePartialSum(Sb,n+1);

      int Cost[n+1][n+1];
      for (int i=0;i<n;i++)
	{
	  Cost[i][0]= 1000000;
	  Cost[0][i]= 1000000;
	}

      Cost[0][0] = 0;

      for (int i=1;i<n+1;i++)
	for (int j=1;j<n+1;j++)
	  {
	    Cost[i][j]=INT_MAX;
	    for (int ka= 1;ka<=i;ka++)
		{
		  int kb=1;
		  Cost[i][j] = std::min(Cost[i][j] , Cost[i-ka][j-kb] + (((Sa[i] - Sa[i-ka]))*((Sb[j] - Sb[j-kb]))));
		}

	    for (int kb= 1;kb<=j;kb++)
	      {
		int ka=1;
		Cost[i][j] = std::min(Cost[i][j], Cost[i-ka][j-kb] + (((Sa[i] - Sa[i-ka]))*((Sb[j] - Sb[j-kb]))));
	      }
	  }

      cout<<Cost[n][n]<<endl;
    }
}