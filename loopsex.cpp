//sum of odd numbers from 1 to 15
#include<iostream>
using namespace std;
int main()
{
    int i,sum=0;
    
    for(i=1;i<=15;i++)
    {
    if(i%2!=0)
    {
        cout<<i<<endl;
        sum=sum+i;
    }
    }
    cout<<"Sum is: "<<sum<<endl;
}