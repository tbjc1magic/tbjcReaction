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
// $Id: tbjcPrimaryGeneratorAction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file tbjcPrimaryGeneratorAction.hh
/// \brief Definition of the tbjcPrimaryGeneratorAction class

#ifndef tbjcPrimaryGeneratorAction_h
#define tbjcPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
class G4IonTable;
class TBJCBEAM;
class G4ParticleGun;
class G4Event;
class G4Box;
class KINECAL;
class G4ParticleDefintion;
class BEAMCOMPGENERATOR;
class BEAMCOMPINFO;
class ATOMICMASSTABLE;
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

class tbjcPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    static:: tbjcPrimaryGeneratorAction* generator;

    tbjcPrimaryGeneratorAction();    
    virtual ~tbjcPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
 
    ////generate parameters/////

    G4ThreeVector startpoint;
    int generateType;
    double Kr;//unit:MeV
    double mass0,mass1,mass2,mass3; //unit:u
    double K0; //unit:MeV
    G4double Ex; //unit:MeV;
    KINECAL *kin; 
    G4IonTable* tbjctable;
    G4ParticleDefinition* beamION;
    G4ParticleDefinition* recoil;
    G4ParticleDefinition* light;
    
    BEAMCOMPGENERATOR* tbjcbg;
    BEAMCOMPGENERATOR* tbjcrg;
    BEAMCOMPGENERATOR* tbjclg;
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    TBJCBEAM* tbjcbeamer;
    G4Box* fEnvelopeBox;
    
    ATOMICMASSTABLE* masstable;

    int cyclecounts;



    int generateP0(const BEAMCOMPINFO * const pickinfo);
    int generateP1(const BEAMCOMPINFO * const  pickinfo);
    int generateP2(const BEAMCOMPINFO * const  pickinfo);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


