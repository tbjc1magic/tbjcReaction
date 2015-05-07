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
// $Id: tbjcEventAction.hh 75216 2013-10-29 16:08:11Z gcosmo $
//
/// \file tbjcEventAction.hh
/// \brief Definition of the tbjcEventAction class

#ifndef tbjcEventAction_h
#define tbjcEventAction_h 1
#include "tbjcDetectorConstruction.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
/// Event action class
///
#include "G4SystemOfUnits.hh"
#include <iostream>
using namespace std;
class tbjcEventAction : public G4UserEventAction
{
  public:

      static:: tbjcEventAction* EventAction;

    tbjcEventAction();
    virtual ~tbjcEventAction();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddSIEdep(G4double edep) { fSIEdep += edep; }
    void updateSiPosition(G4ThreeVector a){SiPos=a;}
   void updateenergy(G4double tempenergy){tbjcEnergy=tempenergy;}
   void updateIonEnergies(double IonEnergy, int SecNum){tbjcIonEnergies[SecNum]+=IonEnergy;}
    G4ThreeVector getSiPos(){return SiPos;}
    double getEnergy(){return tbjcEnergy;  }
    void updateBeamType(int BT){BeamType=BT;}
    void updatetest(double BT){if(tbjctest<0.1*MeV)tbjctest=BT;}
    void updateDT(double DT){tbjcDtotal+=DT;}
    void updateSIenergy(double SIe){tbjcSIenergy=SIe;}
    void updateCMSangle(double theta, double phi){thetaCMS=theta; phiCMS=phi;}
  private:
    G4ThreeVector SiPos;
    G4double  fSIEdep;
    G4double  tbjcEnergy;
    double tbjcIonEnergies[IonSecNum];
    int BeamType;
    double tbjctest;
    double tbjcDtotal;
    int blocked;
    double tbjcSIenergy;
    int print();
    int zeroForEveryParticle();

    double thetaCMS;
    double phiCMS;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
