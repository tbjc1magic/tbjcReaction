//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: tbjcEventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file tbjcEventAction.cc
/// \brief Implementation of the tbjcEventAction class
#include "tbjcglobals.hh"
#include "tbjcEventAction.hh"
#include "tbjcSteppingAction.hh"
#include "tbjcRun.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "DATARECORDER.hpp"
#include <fstream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcEventAction* tbjcEventAction::EventAction=0;

ofstream aa1("energydeposit.dat");
    tbjcEventAction::tbjcEventAction()
: G4UserEventAction(),
    fSIEdep(0.),tbjcEnergy(0)
{

    EventAction=this;
    BeamType=-1;
//    zeroForEveryParticle();
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcEventAction::~tbjcEventAction()
{


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcEventAction::BeginOfEventAction(const G4Event*)
{  

}

int tbjcEventAction::print()
{

    cout<<BeamType<<" blocked "<<blocked<<endl; 
    cout<<"Ionization energy"<<tbjcIonEnergies[0]/MeV<<" "<<tbjcIonEnergies[1]/MeV<<" "<<tbjcIonEnergies[2]/MeV<<" "<<tbjcIonEnergies[3]/MeV<<" "<<tbjcIonEnergies[4]/MeV<<" "<<tbjcEnergy<<" "<<BeamType<<" blocked "<<blocked<<endl;






}

int tbjcEventAction::zeroForEveryParticle()
{
    SiPos =G4ThreeVector(0,0,0);
    fSIEdep=0;

    for(int i=0;i<IonSecNum;i++)
    {
        tbjcIonEnergies[i]=0; 
    }

    tbjcDtotal=0;
    tbjcEnergy=0;
    BeamType=-1;

    thetaCMS = -1;
    phiCMS = -1;
   
    tbjcSteppingAction::STaction->zerotbjcForEveryParticle();


}


ofstream testf("testf.dat");
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcEventAction::EndOfEventAction(const G4Event*)
{
   tbjcSteppingAction* stepaction = tbjcSteppingAction::STaction;
   /*
    testf<<stepaction->preK<<" "<<stepaction->postK<<" "<<stepaction->reactionK<<" "
            <<stepaction->prepoint.x()<<" "<<stepaction->prepoint.y()<<" "<<stepaction->prepoint.z()<<" "
            <<stepaction->postpoint.x()<<" "<<stepaction->postpoint.y()<<" "<<stepaction->postpoint.z()<<" "
            <<stepaction->lastpoint.x()<<" "<<stepaction->lastpoint.y()<<" "<<stepaction->lastpoint.z()<<" "
            <<endl;
*/
/*
   if(!tbjcSteppingAction::STaction->reactionHappened)testf<<BeamType<<" "<<stepaction->lastpoint.x()<<" "<<stepaction->lastpoint.y()<<" "<<stepaction->lastpoint.z()<<" "<<stepaction->reactionK
            <<endl;

*/

    SiPos = tbjcSteppingAction::STaction->GetSIposition();
    blocked = tbjcSteppingAction::STaction->GetBlocked();
    tbjcSIenergy = tbjcSteppingAction::STaction->GetSIenergy(); 


    G4ThreeVector BlockedPosition = tbjcSteppingAction::STaction->GetBlockedPosition();
    double* IonEnergy = tbjcSteppingAction::STaction->GetIonEnergy();
    
/*
    for(int i=0;i<IonSecNum;i++)
    {
        tbjcIonEnergies[i] =  IonEnergy[i];
    }

    double total=0;
    for(int i=0;i<IonSecNum;i++)
    {
        total+=tbjcIonEnergies[i];
    }


    aa1<<BeamType<<" ";
    for(int i=0;i<IonSecNum;i++)
    {
        aa1<<tbjcIonEnergies[i]<<" ";
    }
    aa1<<" "<<SiPos.x()<<" "<<SiPos.y()<<" "<<SiPos.z()<<" ";
    aa1<<tbjcEnergy<<" "<<blocked<<" "<<tbjcSIenergy;
    aa1<<endl;
*/


    DATARECORDER* DataRecorder = DATARECORDER:: DataRecorder;
    
    double* tbjcIonEnergies_recorder;
    switch(BeamType)
    {
        case 0:
            DataRecorder->Write2File();
            DataRecorder->tbjcReactionHappened = tbjcSteppingAction::STaction->reactionHappened;
            tbjcIonEnergies_recorder = DataRecorder->tbjcBeamIonEnergies;            
            DataRecorder->tbjcEnergy = tbjcEnergy;
            DataRecorder->tbjcBlockedPosition_Beam = BlockedPosition;
            break;
        case 1:
            tbjcIonEnergies_recorder = DataRecorder->tbjcLightIonEnergies;            
            DataRecorder->SiPos = SiPos;
            //cout<<SiPos.x()<<" "<<SiPos.y()<<" "<<SiPos.z()<<" "<<tbjc
            DataRecorder->tbjcSIenergy = tbjcSIenergy;
            DataRecorder->tbjcBlockedPosition_Light = BlockedPosition;
            break;
        case 2:
            tbjcIonEnergies_recorder = DataRecorder->tbjcRecoilIonEnergies;            
            DataRecorder->tbjcBlockedPosition_Recoil = BlockedPosition;
            DataRecorder->thetaCMS = thetaCMS;
            DataRecorder->phiCMS = phiCMS;
            break;    
    }

    for(int i=0;i<IonSecNum;i++) tbjcIonEnergies_recorder[i] = IonEnergy[i];
    



    zeroForEveryParticle();
 
    
//    blocked = tbjcSteppingAction::STaction->zerotbjcForEveryParticle();



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
