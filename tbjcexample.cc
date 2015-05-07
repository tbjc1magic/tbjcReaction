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
// $Id: exampletbjc.cc 75216 2013-10-29 16:08:11Z gcosmo $
//
/// \file exampletbjc.cc
/// \brief Main program of the tbjc example

#include "tbjcDetectorConstruction.hh"
#include "tbjcActionInitialization.hh"
#include "tbjcPhysicsList.hh"
//#undef G4MULTITHREADED

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "QGSP_BERT_HP.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4EmStandardPhysics.hh"
#include "G4PhysListFactory.hh"
#include "G4VUserPhysicsList.hh"
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include "TBJCINTERFACE.hpp"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

    TBJCINTERFACE* interface = new TBJCINTERFACE();
    interface->ReadParameterFile("Configuration.in");

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  //
 
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(1);  
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new tbjcDetectorConstruction());
//  runManager->SetUserInitialization(new QBBC);


//G4PhysListFactory *physListFactory = new G4PhysListFactory(); 

//G4VUserPhysicsList *physicsList = 
//
 //    physListFactory->GetReferencePhysList("QGSP_BERT"); 


  //  G4VUserPhysicsList*physicsList = new G4EmStandardPhysics();


 // runManager->SetUserInitialization(physicsList );
  runManager->SetUserInitialization(new tbjcPhysicsList());
//  
  // Physics list
//  G4VModularPhysicsList* physicsList = new QBBC;
  //physicsList->SetVerboseLevel(1);
//runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new tbjcActionInitialization());

  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
