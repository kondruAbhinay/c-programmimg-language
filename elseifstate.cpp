#include<iostream>
using namespace std;
int main()
{
    int money;
    cout<<"how much money you have:";
    cin>>money;
    if(money>=1000)
    cout<<"let's get some coffee in starbucks"<<endl;
    else if(money>=500)
    cout<<"let's get some coffee in cafe coffee day"<<endl;
    else if(money>=100)
    cout<<"let's get some coffee in local shop"<<endl;
    else
    cout<<"let's go to home"<<endl;
    cout<<"and lets go to home."<<endl;
}