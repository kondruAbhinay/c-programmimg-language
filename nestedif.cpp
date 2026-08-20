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
    if(money>=1000)
    {
        if(age>=15)
        {
            cout<<"let's get some coffee"<<endl;
        }
        else
        {
            cout<<"you are not allowed to get coffee"<<endl;
        }
    }
    else
    {
        cout<<"you don't have enough money to get coffee"<<endl;
    }
    cout<<"let's go to home"<<endl;
    return 0;
}
