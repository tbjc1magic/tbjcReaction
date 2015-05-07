#ifndef __BEAMCOMPGENERATOR__
#define __BEAMCOMPGENERATOR__
#include "BEAMCOMP.hpp"
#include <vector>
class BEAMCOMPGENERATOR
{
    public:

        BEAMCOMPGENERATOR();
        int addComp(int Z, int A,int Charge, double Ex, double K,double Chance);
        BEAMCOMP* generateBeam();

    private:
        std::vector<BEAMCOMP> BeamCompContainer;
};

#endif 
