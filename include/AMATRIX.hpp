#ifndef __AMATRIX__
#define __AMATRIX__
#include <Eigen/Core>
using Eigen::Matrix4d;
class FOURVECT;
class AMATRIX
{

    friend class FOURVECT;

    public:
        AMATRIX(float beta=0);
        ~AMATRIX();
        int setBeta(float beta); 
        int reverseBeta();
    private:
        Matrix4d *A;
};


#endif
