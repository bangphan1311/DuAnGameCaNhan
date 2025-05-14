#include<iostream>
#include<vector>

using namespace std;
char a[30];

string s;
void sinh(int b , int n)
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
 for(int t=0 ;t < s.size() ; t++)
 {
     a[b]=s[t];
     sinh(b+1 , n);
 }
 }


int main()
{
    int n; cin>>n;
    cin >>s;
    for(int i=1 ; i<=n ; i++)
    {
        sinh(0 , i);
    }
}

