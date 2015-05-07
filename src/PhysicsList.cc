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
/// \file radioactivedecay/rdecay01/src/PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//
//
// $Id: PhysicsList.cc 73284 2013-08-23 08:35:02Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "PhysicsList.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleTypes.hh"
#include "G4IonConstructor.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"
#include "G4SystemOfUnits.hh"
#include "tbjcPhysListEmStandard.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4KleinNishinaModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"
#include "G4SystemOfUnits.hh"
#include "G4BraggIonModel.hh"





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
: G4VUserPhysicsList()
{
  //add new units for radioActive decays
  // 
  const G4double minute = 60*second;
  const G4double hour   = 60*minute;
  const G4double day    = 24*hour;
  const G4double year   = 365*day;
  new G4UnitDefinition("minute", "min", "Time", minute);
  new G4UnitDefinition("hour",   "h",   "Time", hour);
  new G4UnitDefinition("day",    "d",   "Time", day);
  new G4UnitDefinition("year",   "y",   "Time", year);        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  
  // gamma
  G4Gamma::GammaDefinition();

  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  
  // baryons
  G4Proton::ProtonDefinition();
  G4Neutron::NeutronDefinition();  

  // ions
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  
  AddTransportation();
  G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  //Caution! With G4MT migration this threshold can no longer be set smaller
  //than nanosecond
  radioactiveDecay->SetHLThreshold(nanosecond);

  radioactiveDecay->SetICM(true);                //Internal Conversion
  radioactiveDecay->SetARM(false);               //Atomic Rearangement
  
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();  
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());
      
  // Deexcitation (in case of Atomic Rearangement)
  //
  G4UAtomicDeexcitation* de = new G4UAtomicDeexcitation();
  de->SetFluo(true);
  de->SetAuger(true);   
  de->SetPIXE(false);  
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);  





      

            ////ph->RegisterProcess(new G4RayleighScattering, particle);      
            ph->RegisterProcess(new G4PhotoElectricEffect, G4Gamma::Gamma()  );      
            G4ComptonScattering* cs   = new G4ComptonScattering;
            cs->SetEmModel(new G4KleinNishinaModel());//G4hBetheBlochModel
            ph->RegisterProcess(cs, G4Gamma::Gamma() );
            ph->RegisterProcess(new G4GammaConversion, G4Gamma::Gamma() );


            ph->RegisterProcess(new G4eMultipleScattering(),G4Electron::Electron());
            //            
            G4eIonisation* eIoni = new G4eIonisation();
            eIoni->SetStepFunction(0.1, 100*um);      
            ph->RegisterProcess(eIoni, G4Electron::Electron()  );
            //
            ph->RegisterProcess(new G4eBremsstrahlung(), G4Electron::Electron()  );      
         
            
            ph->RegisterProcess(new G4eMultipleScattering(), G4Positron::Positron());
            //  
            G4eIonisation* pIoni = new G4eIonisation();
            pIoni->SetStepFunction(0.1, 100*um);  
            ph->RegisterProcess(pIoni, G4Positron::Positron());
            //
            ph->RegisterProcess(new G4eBremsstrahlung(), G4Positron::Positron());
            //
            ph->RegisterProcess(new G4eplusAnnihilation(), G4Positron::Positron());
/*
        } else if (particleName == "mu+" || 
                particleName == "mu-"    ) {

            ph->RegisterProcess(new G4MuMultipleScattering(), particle);
            G4MuIonisation* muIoni = new G4MuIonisation();
            muIoni->SetStepFunction(0.1, 50*um);      
            ph->RegisterProcess(muIoni, particle);
            ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
            ph->RegisterProcess(new G4MuPairProduction(), particle);

        } else if( particleName == "proton" ||
                particleName == "pi-" ||
                particleName == "pi+"    ) {

            ph->RegisterProcess(new G4hMultipleScattering(), particle);      
            G4hIonisation* hIoni = new G4hIonisation();
            hIoni->SetStepFunction(0.1, 20*um);
            ph->RegisterProcess(hIoni, particle);
            ph->RegisterProcess(new G4hBremsstrahlung(), particle);
            ph->RegisterProcess(new G4hPairProduction(), particle);            

        } else if( particleName == "alpha" || 
                particleName == "He3"    ) {

          //  ph->RegisterProcess(new G4hMultipleScattering(), particle);           

            G4ionIonisation* ionIoni = new G4ionIonisation();
            ionIoni->SetStepFunction(0.1, 1*um);
        //     ionIoni->SetEmModel(new G4hBetheBlochModel());//G4hBetheBlochModel

        //      ionIoni->SetEmModel(new G4hNuclearStoppingModel("nuclearstopping"));//G4hBetheBlochModel
        //    ionIoni->SetEmModel(new G4BetheBlochModel());
        //    ionIoni->SetEmModel(new G4BraggIonModel());
            ph->RegisterProcess(ionIoni, particle);
       //     ph->RegisterProcess(new G4NuclearStopping(), particle);      
*/

            ph->RegisterProcess(new G4hMultipleScattering(), G4GenericIon::GenericIon());          
            G4ionIonisation* ionIoni = new G4ionIonisation();
            ionIoni->SetEmModel(new G4IonParametrisedLossModel());
            ionIoni->SetStepFunction(0.1, 1*um);
            ph->RegisterProcess(ionIoni, G4GenericIon::GenericIon());
            ph->RegisterProcess(new G4NuclearStopping(), G4GenericIon::GenericIon());                   


       
   




}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutsWithDefault();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
