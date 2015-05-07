#include "FOURVECT.hpp"
#include "AMATRIX.hpp"
#include <iostream>
using namespace std;
FOURVECT::FOURVECT(double comp1,double comp2,double comp3,double comp4)
{
//    cout<<"parent constructor"<<endl;
    v= new Vector4d;
    (*v)(0)=comp1;
    (*v)(1)=comp2;
    (*v)(2)=comp3;
    (*v)(3)=comp4;
}




FOURVECT::~FOURVECT()
{
//    cout<<"this is the parent destructor"<<endl;
    delete v;
}

int FOURVECT::print()
{
    cout<<(*v)<<endl;

    return 1;
}

static int sum(FOURVECT* vect1, FOURVECT* vect2, FOURVECT* vect3)
{


    return 1;
}

int FOURVECT::boost(AMATRIX * A)
{

    (*v)=(*(A->A))*(*v);
//    cout<<(*v)<<endl;
    return 1;
}





FOURVECT FOURVECT:: operator + (FOURVECT &a)
{
    FOURVECT aa;
   
    aa.setP4((*v)(0)+a(0),(*v)(1)+a(1),(*v)(2)+a(2),(*v)(3)+a(3));


    return aa;
}


int FOURVECT:: setP4(double P0,double P1,double P2,double P3)
{
    (*v)(0)=P0;
    (*v)(1)=P1;
    (*v)(2)=P2;
    (*v)(3)=P3;

    return 1;
}




void FOURVECT:: operator = (FOURVECT a)
{
    (*v)(0)=a(0);
    (*v)(1)=a(1);
    (*v)(2)=a(2);
    (*v)(3)=a(3);

}

double FOURVECT::operator ()(int index)
{

    return (*v)(index);
}
