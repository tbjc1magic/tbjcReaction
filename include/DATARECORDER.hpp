#ifndef __DATARECORDER__
#define __DATARECORDER__
#include <string>
#include <fstream>
#include "tbjcglobals.hh"
#include "G4ThreeVector.hh"
using namespace std;


class DATARECORDER
{

    public:
        DATARECORDER();
        ~DATARECORDER();

        int OpenFile(string fname);
        int Write2File();

        ////public members////////
        ofstream foutput;

        static DATARECORDER* DataRecorder;
        static int Initialization();

        int tbjcReactionHappened;
        double tbjcBeamIonEnergies[IonSecNum];
        double tbjcRecoilIonEnergies[IonSecNum];
        double tbjcLightIonEnergies[IonSecNum];
        G4ThreeVector SiPos;
        double tbjcEnergy;
        double tbjcSIenergy;
        G4ThreeVector tbjcBlockedPosition_Beam;
        G4ThreeVector tbjcBlockedPosition_Recoil;
        G4ThreeVector tbjcBlockedPosition_Light;
    
        double thetaCMS, phiCMS;

    private:

        ////private function////////
        int reset();
};


#endif

