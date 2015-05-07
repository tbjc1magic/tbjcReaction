#ifndef __BEAMCOMPINFO__
#define __BEAMCOMPINFO__
#include "G4ThreeVector.hh"
class BEAMCOMP;
class BEAMCOMPINFO
{

    public:
        BEAMCOMP* ParticleComp;
        float K;
        G4ThreeVector ParticleDirection;
        G4ThreeVector ParticlePosition;
};

#endif

