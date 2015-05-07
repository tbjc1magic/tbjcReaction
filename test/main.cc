#include <iostream>
#include "BEAMCOMP.hpp"
#include "BEAMCOMPGENERATOR.hpp"
#include "TH1.h"
#include "TFile.h"
using namespace std;

int main()
{

    TFile* f = new TFile("output.root","recreate");
    TH1F* dist = new TH1F("dist","dist",600,-1,5);

    BEAMCOMPGENERATOR* tbjcrg = new BEAMCOMPGENERATOR;
    
    tbjcrg->addComp(10,21,10,0,0,0.1);
    tbjcrg->addComp(10,21,10,0.3788,0,1);
    tbjcrg->addComp(10,21,10,1.745911,0,0.1);
    tbjcrg->addComp(10,21,10,2.79416 ,0,1);
    tbjcrg->addComp(10,21,10,3.73559 ,0,0.3);
    tbjcrg->addComp(10,21,10,4.52586 ,0,0.5);
    tbjcrg->addComp(10,21,10,4.72534 ,0,0.5);

    for(int i=0;i<10000;i++)
    {
        BEAMCOMP*pick = tbjcrg->generateBeam(); 
        dist->Fill(pick->Ex);
    }

    f->Write();

    return 1;

}
