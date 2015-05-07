#ifndef __FOURVECT__
#define __FOURVECT__
#include <Eigen/Dense>
using Eigen::Vector4d;
class  AMATRIX;
class FOURVECT
{

    public:
        FOURVECT(double c1=0, double c2=0, double c3=0, double c4=0);
        ~FOURVECT();
        int boost(AMATRIX * A);
        int  print();

        int setP4(double P0,double P1,double P2,double P3);

      double operator ()(int index);
      FOURVECT operator + (FOURVECT &a);
      void  operator = (FOURVECT a);



    protected:
        Vector4d *v;
};



#endif
