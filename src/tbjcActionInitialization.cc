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
// $Id: tbjcActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file tbjcActionInitialization.cc
/// \brief Implementation of the tbjcActionInitialization class

#include "tbjcActionInitialization.hh"
#include "tbjcPrimaryGeneratorAction.hh"
#include "tbjcRunAction.hh"
#include "tbjcEventAction.hh"
#include "tbjcSteppingAction.hh"
#include "tbjcDetectorConstruction.hh"
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcActionInitialization::tbjcActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcActionInitialization::~tbjcActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcActionInitialization::BuildForMaster() const
{
  SetUserAction(new tbjcRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcActionInitialization::Build() const
{
  SetUserAction(new tbjcPrimaryGeneratorAction);
  SetUserAction(new tbjcRunAction);
  
  tbjcEventAction* eventAction = new tbjcEventAction;
  SetUserAction(eventAction);

  tbjcSteppingAction* STAction = new tbjcSteppingAction(eventAction);
  SetUserAction(STAction);
    
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
