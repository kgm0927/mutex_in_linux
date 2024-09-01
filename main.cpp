#include <iostream>
#include <unistd.h>

using namespace std;

class reference{
    
    public:
    int i;

    reference* returning(){
        return this;
    }

    void show_i(){
        cout<<"i:"<<i<<endl;
    }

};

int main(){
    reference i;
    i.i=5;

    reference* ii=i.returning();

    cout<<"i.i"<<i.i<<endl;
    cout<<"ii.i"<<ii->i<<endl;


    ii->i=10;

    cout<<"i.i"<<i.i<<endl;
    cout<<"ii.i"<<ii->i<<endl;

    i.i=9;

    cout<<"i.i:"<<i.i<<endl;
    cout<<"ii.i:"<<ii->i<<endl;

}