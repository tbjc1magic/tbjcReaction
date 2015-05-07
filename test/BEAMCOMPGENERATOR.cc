#include "BEAMCOMPGENERATOR.hpp"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;


BEAMCOMPGENERATOR::BEAMCOMPGENERATOR()
{
    srand(time(0));
}

BEAMCOMP* BEAMCOMPGENERATOR::generateBeam()
{

    double totalChance=0;

    for(vector<BEAMCOMP>::iterator it = BeamCompContainer.begin(); it != BeamCompContainer.end(); ++it)
    {
        totalChance += it->Chance;
    }


    double bobo = rand()%(int)(10000*totalChance)/10000.0; 

    for(vector<BEAMCOMP>::iterator it = BeamCompContainer.begin(); it != BeamCompContainer.end(); ++it)
    {
        bobo -= it->Chance;
        if(bobo<0)return &(*it);
    }

    return 0;

}


int BEAMCOMPGENERATOR::addComp(int Z, int N,int Charge ,double Ex,double K, double Chance)
{
    BeamCompContainer.push_back(BEAMCOMP(Z,N,Charge,Ex,K,Chance));
    return 1;
}
