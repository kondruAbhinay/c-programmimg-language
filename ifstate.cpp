#include<iostream>
using namespace std;
int main()

{
    int  age,money;
    cout<<"how much money you have:";
    cin>>money;
    cout<<endl;
    cout<<"what is your age:";
    cin>>age;
    cout<<endl;
    if(money>=1000   && (age>=15))
    cout<<"let's get some coffee"<<endl;
    cout<<"let's go to home"<<endl;
    return 0;
}