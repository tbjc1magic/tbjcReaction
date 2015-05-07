#include "P4VECTOR.hpp"
#include "constants.hpp"
#include "FOURVECT.hpp"
#include "AMATRIX.hpp"
#include <iostream>
using namespace std;

P4VECTOR:: P4VECTOR(double m, double v0)
    ////create a P4 vector using initial speed, only P4_0,P4_3 have value; unit: kg*m/s
{

 //   cout<<"this is the child constructor"<<endl;


    if(v0>=constant_c)
    {

        cout<<"what the heck are u doing?"<<endl;
        return ;
    }

    double beta_tmp=v0/constant_c;
    double gamma_tmp=1/sqrt(1-beta_tmp*beta_tmp); 

    double E=m*constant_c*gamma_tmp;
    double p3=m*v0*gamma_tmp;

    (*v)(0)=E;
    (*v)(3)=p3;
}

P4VECTOR:: P4VECTOR()
{
   (*v)(0)=(*v)(1)=(*v)(2)=(*v)(3)=0;
}


P4VECTOR:: ~P4VECTOR()
{
//    cout<<"this is the child destructor"<<endl;

}


int P4VECTOR::boost(AMATRIX * A)
{
    FOURVECT::boost(A);
}



