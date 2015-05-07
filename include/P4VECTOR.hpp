#ifndef __P4VECTOR__
#define __P4VECTOR__

#include "FOURVECT.hpp"

class AMATRIX;
class P4VECTOR: public FOURVECT
{

    public:
        P4VECTOR(double m, double v0);
        P4VECTOR();
        ~P4VECTOR();
        int boost(AMATRIX * A);

        using FOURVECT:: operator = ;


    private:


};

#endif
