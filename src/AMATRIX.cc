#include "AMATRIX.hpp"
#include <iostream>
#include <math.h>

using namespace std;



AMATRIX::AMATRIX(float beta)
{
    A=new Matrix4d;

    setBeta(beta);

 //   cout<<(*A)<<endl;
}



AMATRIX::~AMATRIX()
{
    delete A;

}

int AMATRIX::reverseBeta()
{
    (*A)(0,3)=(*A)(3,0)=-(*A)(3,0);
    return 1;
}

int AMATRIX::setBeta(float beta)
{
    int dim=4;
    for(int i=0;i<dim;i++)
    {
        for(int j=0;j<dim;j++)
        {
            (*A)(i,j)=0; 
        }
    }

    if(fabs(beta)>=1)
    {
        cout<<"what the heck r u doing?"<<endl;
        return 0;
    }

    float gamma=1/sqrt(1-beta*beta);
    (*A)(0,0)=(*A)(3,3)=gamma;
    (*A)(1,1)=(*A)(2,2)=1;
    (*A)(0,3)=(*A)(3,0)=-gamma*beta;


    return 1;
}
