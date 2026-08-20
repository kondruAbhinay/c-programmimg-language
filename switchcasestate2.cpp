#include<iostream>
using namespace std;
int main()
{
    int choice;
    cout<<"enter your choice:"<<endl;
    cin>>choice;
    switch(choice)
    {
        case 1:
        cout<<"you have selected 1"<<endl;
        break;
        case 2:
        cout<<"you have selected 2"<<endl;
        break;
        case 3:
        cout<<"you have selected 3"<<endl;
        break;
        default:
        cout<<"you have selected wrong choice"<<endl;
    }
    cout<<"thank you for your choice"<<endl;
}