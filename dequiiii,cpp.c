#include<iostream>
#include<vector>

using namespace std;
int a[30];
int n;

void sinh(b)
{
 if(b==n)
 {
 for(int i=0 ; i< n ; i++)
 {
     cout<<a[i];
 }
 cout<<endl;
 return ;
 }
 for(int t=0 ; t<2 ; t++)
 {
     a[b]=t;
     sinh(b+1);

 }
 }


int main()
{
   cin>>n;
    sinh(0);
}

