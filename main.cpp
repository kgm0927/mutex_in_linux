#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

using namespace std;


int main(){

    int *i=new int(5);
    
    delete i;


    cout<<"i:"<<i<<endl;


}