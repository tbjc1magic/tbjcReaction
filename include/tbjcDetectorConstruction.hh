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
// $Id: tbjcDetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file tbjcDetectorConstruction.hh
/// \brief Definition of the tbjcDetectorConstruction class

#ifndef tbjcDetectorConstruction_h
#define tbjcDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "tbjcglobals.hh"
#include "tbjcMagneticField.hh"
#include "G4FieldManager.hh"
#include "G4Material.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;
class tbjcSteppingAction;
/// Detector construction class to define materials and geometry.

class tbjcDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        tbjcDetectorConstruction();
        virtual ~tbjcDetectorConstruction();

        virtual G4VPhysicalVolume* Construct();
        

        tbjcMagneticField* fMagneticField;
        G4FieldManager* fFieldMgr;
        G4LogicalVolume*  TargetVolume;
        double TargetShift;
        static::tbjcDetectorConstruction*Detector;
        G4LogicalVolume* SIdetector;
        G4LogicalVolume* HELIOSBlocker;
        G4LogicalVolume **Ionsections;

        G4LogicalVolume* ballouttest;
        G4LogicalVolume* ballouttest2;

    private:

        double HELIOSradius;
        double HELIOSlength;
        double HELIOSshellThickness;
       

        
        G4VPhysicalVolume* constructSTinHELIOS(G4LogicalVolume* logicParent);  
        
        G4VPhysicalVolume* constructTarget(G4LogicalVolume* logicParent);  
        G4VPhysicalVolume* constructBlocker(G4LogicalVolume* logicParent);  
        G4VPhysicalVolume* constructSiliconArray(G4LogicalVolume* logicParent);  
        G4VPhysicalVolume* constructIonFront(G4LogicalVolume* logicParent);  
        G4VPhysicalVolume* constructIonInside(G4LogicalVolume* logicParent);  
        G4VPhysicalVolume* constructHELIOSshell(G4LogicalVolume* logicParent);  
        int tbjcMaterialsConstruction();

        G4Material* tbjcbutane;
        G4Material* tbjcH2O;
        G4Material* tbjcCF4;
        G4Material* tbjcVacuum;
        G4Material* world_mat;
        G4Material* tbjcSi;
        G4Material* tbjcAl;
        G4Material* tbjcFe;
        G4Material* tbjcKapton;
        G4Material* tbjcHe2;
        G4Material* tbjcCD2;
        G4Material* tbjcCu;
        
//    protected:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

