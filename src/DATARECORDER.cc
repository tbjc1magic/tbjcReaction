#include "DATARECORDER.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


DATARECORDER*DATARECORDER::DataRecorder=0;

int DATARECORDER::Initialization()
{
    DATARECORDER* tmp = new DATARECORDER();
    DATARECORDER::DataRecorder = tmp;
}




DATARECORDER::DATARECORDER()
{
    reset();
}


int DATARECORDER:: reset()
{
    tbjcReactionHappened = 0;


    for(int i =0; i<IonSecNum; i++)
    {
        tbjcBeamIonEnergies[i]=0;
        tbjcRecoilIonEnergies[i]=0;
        tbjcLightIonEnergies[i]=0;
    }

    SiPos = G4ThreeVector(0,0,0);
    tbjcEnergy = 0;
    tbjcSIenergy=0;
   
    tbjcBlockedPosition_Beam = G4ThreeVector(0,0,0);
    tbjcBlockedPosition_Recoil = G4ThreeVector(0,0,0);
    tbjcBlockedPosition_Light = G4ThreeVector(0,0,0);
    

    thetaCMS = phiCMS = 0;

    return 1;
}


DATARECORDER::~DATARECORDER()
{
    foutput.close();



}

int DATARECORDER::OpenFile(string fname)
{

    foutput.open (fname.c_str(), std::ofstream::out );

    return 1;
}


int DATARECORDER::Write2File()
{

    foutput<<tbjcReactionHappened<<" ";
    
    for(int i=0;i<IonSecNum;i++) foutput<< tbjcBeamIonEnergies[i]<<" ";
    for(int i=0;i<IonSecNum;i++) foutput<< tbjcRecoilIonEnergies[i]<<" ";
    for(int i=0;i<IonSecNum;i++) foutput<< tbjcLightIonEnergies[i]<<" ";
    
    foutput<<SiPos.x()<<" ";
    foutput<<SiPos.y()<<" ";
    foutput<<SiPos.z()<<" ";

    foutput<<tbjcEnergy<<" ";
    foutput<<tbjcSIenergy<<" ";


    foutput<<tbjcBlockedPosition_Beam.x()<<" ";
    foutput<<tbjcBlockedPosition_Beam.y()<<" ";
    foutput<<tbjcBlockedPosition_Beam.z()<<" ";

    foutput<<tbjcBlockedPosition_Recoil.x()<<" ";
    foutput<<tbjcBlockedPosition_Recoil.y()<<" ";
    foutput<<tbjcBlockedPosition_Recoil.z()<<" ";


    foutput<<tbjcBlockedPosition_Light.x()<<" ";
    foutput<<tbjcBlockedPosition_Light.y()<<" ";
    foutput<<tbjcBlockedPosition_Light.z()<<" ";

    foutput<<thetaCMS<<" "<<phiCMS<<" ";


    foutput<<endl;

    reset();

    return 1;
}
