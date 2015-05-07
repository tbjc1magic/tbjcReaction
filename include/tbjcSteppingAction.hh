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
// $Id: tbjcSteppingAction.hh 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file tbjcSteppingAction.hh
/// \brief Definition of the tbjcSteppingAction class

#ifndef tbjcSteppingAction_h
#define tbjcSteppingAction_h 1
#include "tbjcDetectorConstruction.hh"
#include "G4UserSteppingAction.hh"
#include "tbjcglobals.hh"
#include "G4LogicalVolume.hh"
#include "tbjcEventAction.hh"
class tbjcEventAction;
class tbjcDetectorConstruction;
class G4LogicalVolume;

/// Stepping action class
/// 

class tbjcSteppingAction : public G4UserSteppingAction
{
  public:

    /////////////////////////////////
    //
    //  tbjc parameters from here
    //
    //////////////////////////////////
    
    G4ThreeVector prepoint;
    G4ThreeVector postpoint;
    G4ThreeVector lastpoint;
    float reactionK;
    float preK,postK;






    double travelLength;
    int reactionHappened;
    double reactionlifeLength;
    int zerotbjc();
    int zerotbjcForEveryParticle();
    //////////////////////////////////
    //
    //
    //////////////////////////////////
    static:: tbjcSteppingAction*STaction;



    tbjcSteppingAction(tbjcEventAction* eventAction);
    virtual ~tbjcSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

    double *GetIonEnergy(){return IonEnergy;}
    G4ThreeVector GetSIposition(){return SIposition;}
    G4ThreeVector GetBlockedPosition(){return BlockedPosition;}
    double GetSIenergy(){return SIenergy;}
    int GetBlocked(){return blocked;}

  private:
    tbjcEventAction*  fEventAction;
    int tbjcStepcounter;


    G4ThreeVector SIposition;
    double SIenergy; 
    double IonEnergy[IonSecNum];
    int blocked;
    G4ThreeVector BlockedPosition;

    G4LogicalVolume** sections;
    G4LogicalVolume* targetVolume;
    G4LogicalVolume* HELIOSBlocker;
    G4LogicalVolume* SIdetector;

    tbjcDetectorConstruction* Detector;
    tbjcEventAction* EventAction;

    int firsttime;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
