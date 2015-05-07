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
// $Id: tbjcSteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file tbjcSteppingAction.cc
/// \brief Implementation of the tbjcSteppingAction class
#include "tbjcPrimaryGeneratorAction.hh"
#include "tbjcSteppingAction.hh"
#include <math.h> 
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include <iostream>
#include <fstream>
#include "G4Track.hh"
#include <fstream>
#include "tbjcExternals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "TBJCINTERFACE.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

tbjcSteppingAction* tbjcSteppingAction:: STaction=0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//ofstream steppoint("steppoint.dat");
//ofstream steppoint1("steppoint1.dat");
G4LogicalVolume* ballouttest;
G4LogicalVolume* ballouttest2;

    tbjcSteppingAction::tbjcSteppingAction(tbjcEventAction* eventAction)
: G4UserSteppingAction(),fEventAction(eventAction)
{


    srand (time(NULL));
    cout<<"heyhey step creater"<<endl;
    tbjcSteppingAction::STaction=this;

    reactionlifeLength = 1*mm*0.1;


    firsttime = 1;


    TBJCINTERFACE::interface->GetAttributes("ReactionLifeLength",(void*)&reactionlifeLength);


}

int tbjcSteppingAction:: zerotbjc()
{

    //    cout<<"zero is coming"<<endl;
    travelLength=0;
    tbjcStepcounter=0;
    reactionHappened=0;
    return 1;
}





int tbjcSteppingAction:: zerotbjcForEveryParticle()
{

    blocked = 0;

    SIposition= G4ThreeVector(0,0,0);
    SIenergy = 0;


    for(int i=0;i<IonSecNum;i++)
    {
        IonEnergy[i]=0; 
    }
    BlockedPosition = G4ThreeVector();
    prepoint = G4ThreeVector();
    postpoint = G4ThreeVector();
    lastpoint = G4ThreeVector();
    preK=0;
    postK=0;
    reactionK=0;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcSteppingAction::~tbjcSteppingAction()
{}





//ofstream testff("testc.dat");
void tbjcSteppingAction::UserSteppingAction(const G4Step* step)
{



    if(firsttime)
    {
        firsttime=0;
        Detector = tbjcDetectorConstruction::Detector; 
        targetVolume = Detector->TargetVolume;

        sections = Detector->Ionsections;
        HELIOSBlocker = Detector->HELIOSBlocker;
        SIdetector = Detector->SIdetector;


    //    EventAction = tbjcEventAction::EventAction; 

        zerotbjc();
        zerotbjcForEveryParticle();


       // ballouttest = Detector->ballouttest; 
       // ballouttest2 = Detector->ballouttest2; 

    }


    // get volume of the current step
    G4LogicalVolume* volume 
        = step->GetPreStepPoint()->GetTouchableHandle()
        ->GetVolume()->GetLogicalVolume();



    int  trackNum = step->GetTrack()->GetTrackID(); 
    double deltadep = step->GetDeltaEnergy();


    //    const G4DynamicParticle* tbjcparticle=step->GetTrack()->GetDynamicParticle();



    if(trackNum==1)
    {
      //  EventAction->updateDT(deltadep);

 //       lastpoint = step->GetPostStepPoint()->GetPosition();
 //       prepoint = step->GetPreStepPoint()->GetPosition();
        reactionK =   step->GetPostStepPoint()->GetKineticEnergy()/MeV;

        //testff<<lastpoint.x()/cm<<" "<<lastpoint.y()/cm<<" "<<lastpoint.z()/cm<<" "<<reactionK/MeV<<endl;
    }





    if(HELIOSBlocker!=0&&volume==HELIOSBlocker&&trackNum==1)
    {
        blocked = 1;

        BlockedPosition = step->GetPostStepPoint()->GetPosition();
     //   step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries); 
    }





    if(volume==SIdetector&&trackNum==1)
    {


        G4ThreeVector position= step->GetPreStepPoint()->GetPosition();
        G4ThreeVector position2= step->GetPostStepPoint()->GetPosition();
        cout<<"displacement:"<<(position.z()-position2.z())/cm<<endl;


        SIposition = position;
        SIenergy =   step->GetPreStepPoint()->GetKineticEnergy()/MeV;

        step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);


    }


    for(int i=0;i<IonSecNum;i++)
    {
        if(volume==sections[i]&&trackNum==1)
        {
            IonEnergy[i]+=deltadep;
        }

    }


    //   if(targetVolume!=0&&volume==targetVolume)cout<<"what the heck"<<endl;



    if(targetVolume!=0&&volume==targetVolume&&trackNum==1&&!reactionHappened)
    {



        double deltaL= step->GetTrack()->GetStepLength();
        travelLength+= deltaL; 

        //    cout<<deltaL/cm*10000<<" um"<<endl;
        double chance = rand() % 10000/10000.0;
        double reaction_trigger = deltaL/reactionlifeLength;

        if(chance<reaction_trigger)
        {
            G4ThreeVector direction = step->GetPreStepPoint()->GetMomentumDirection();
            double Kr =  (step->GetPreStepPoint()->GetKineticEnergy()+step->GetPostStepPoint()->GetKineticEnergy())/2/MeV;
            //   Kr=22;
            tbjcPrimaryGeneratorAction* generator=tbjcPrimaryGeneratorAction::generator;
            generator->startpoint=(step->GetPreStepPoint()->GetPosition()+step->GetPostStepPoint()->GetPosition())/2;
            generator->generateType=2;
            generator->Kr = Kr;
            

            double kk1 = step->GetPreStepPoint()->GetKineticEnergy();
            G4ThreeVector pp1 = step->GetPreStepPoint()->GetPosition();
            double kk2 = step->GetPostStepPoint()->GetKineticEnergy();
            G4ThreeVector pp2 = step->GetPostStepPoint()->GetPosition();

            cout<<"******************"<<endl;
            cout<<deltaL/cm*10000<<endl;
            cout<<kk1<<" "<<pp1.x()<<" "<<pp1.y()<<" "<<pp1.z()<<" "<<endl;
            cout<<kk2<<" "<<pp2.x()<<" "<<pp2.y()<<" "<<pp2.z()<<" "<<endl;

            cout<<"it happends: "<<step->GetPreStepPoint()->GetKineticEnergy()<<" "<<Kr<<" "<<generator->startpoint.z()/cm<<" "<<generator->startpoint.x()/cm<<" "<<generator->startpoint.y()/cm<<endl;
            reactionHappened=1;
        }

    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

