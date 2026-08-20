// C++ program to demonstrate nested if statement by abhi:)
#include<iostream>
using namespace std;
int main()
{
    int money,age;
    cout<<"how much money you have:";
    cin>>money;
    cout<<endl;
    if(money>=1500 && (money<=2000))
    {
        cout<<"enter your age:";
        cin>>age;
        if(age>=25)
        {
            cout<<"let's get some wine"<<endl;
        }
        else if(age>=15 && (age<25))
        {
            cout<<"you are not allowed to get wine,but you can get some coffee"<<endl;
        }
        else
        {
            cout<<"you are not allowed to get wine or coffee,you can have some tea"<<endl;
        }
    }
    if(money>=1000 && (money<1500))
    {
        cout<<"enter your age:";
        cin>>age;
        if(age>=15)
        {
            cout<<"let's get some coffee in sturbucks"<<endl;
        }
        else
        {
            cout<<"you are not allowed to get coffee in sturbucks,you can have some tea"<<endl;
        }
    }
    if(money>=500 && (money<1000))
    {
        cout<<"enter your age:";
        cin>>age;
        if(age>=15)
        {
            cout<<"let's get some coffee in cafe coffee day"<<endl;
        }
        else
        {
            cout<<"you are not allowed to get coffee in cafe coffee day,you can have some tea"<<endl;
        }
    }
    else if(money>=100 && (money<500))
    {
        cout<<"enter your age:";
        cin>>age;
        if(age>=15)
        {
            cout<<"let's get some coffee in local shop"<<endl;
        }
        else
        {
            cout<<"you are not allowed to get coffee in local shop,you can have some tea"<<endl;
        }
    }
    else if(money<100)

    {
        cout<<"lets go home,we don't have enough money!!"<<endl;
    }
    else
    {
        cout<<"lets go!!"<<endl;
    }
}
