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
// $Id: tbjcDetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file tbjcDetectorConstruction.cc
/// \brief Implementation of the tbjcDetectorConstruction class

#include "tbjcDetectorConstruction.hh"


#include <cstring>
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4EllipticalTube.hh"
#include "G4SubtractionSolid.hh"
#include "CADMesh.hh"
#include "G4UnionSolid.hh"
#include "G4UserLimits.hh"
#include "G4UniformMagField.hh"
#include "G4ChordFinder.hh"
#include <iostream>
#include "G4Ellipsoid.hh"
#include "G4Tubs.hh"
#include "TBJCINTERFACE.hpp"
using namespace std;


tbjcDetectorConstruction*  tbjcDetectorConstruction::Detector=0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    tbjcDetectorConstruction::tbjcDetectorConstruction()
: G4VUserDetectorConstruction(),HELIOSradius(46*cm),HELIOSlength(235*cm),HELIOSshellThickness(3*cm)
{
    Detector=this;
    TargetVolume=0;
    SIdetector=0;

    Ionsections = new G4LogicalVolume*[IonSecNum];


}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcDetectorConstruction::~tbjcDetectorConstruction()
{ }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* tbjcDetectorConstruction::Construct()
{ 




    G4double z, a, density;
    density = 0*g/cm3;
    a = 39.95*g/mole;
    world_mat = new G4Material("Vacuum", z=18., a, density);


    G4double tbjca, tbjcz;
    G4double tbjctemperature, tbjcpressure;
    G4int tbjcnel;

    G4Element* tbjcH  = new G4Element("Hydrogen" ,"H" , tbjcz= 1., tbjca=   1.01*g/mole);
    G4Element* tbjcHe  = new G4Element("Helium" ,"He" , tbjcz= 2., tbjca=   2.01*g/mole);

    G4Element* tbjcN  = new G4Element("Nitrogen" ,"N" , tbjcz= 7., tbjca=  14.01*g/mole);
    G4Element* tbjcO  = new G4Element("Oxygen"   ,"O" , tbjcz= 8., tbjca=  16.00*g/mole);
    G4Element* tbjcC  = new G4Element("Carbon"   ,"C" , tbjcz= 6., tbjca=  12.00*g/mole);
    //	G4Element* C  = new G4Element("Carbon"   ,"C" , z= 6., a=  12.00*g/mole);
    G4Element* tbjcAr  = new G4Element("Ar"   ,"Ar" , tbjcz= 18., tbjca=  39.95*g/mole);

    G4Element* tbjcF  = new G4Element("F"   ,"F" , tbjcz= 9., tbjca=  18.998*g/mole);
    G4Element* tbjcD  = new G4Element("D","D", tbjcz = 1., tbjca = 2.0141017926*g/mole);

    //
    // define materials
    //
    G4double tbjcdensity;
    G4int tbjcncomponents, tbjcnatoms;
    G4double tbjcfractionmass;  

    tbjcbutane = 
        new G4Material("tbjcbutane", density= 0.00249343*g/cm3, tbjcncomponents=2);
    tbjcbutane->AddElement(tbjcC, 4);
    tbjcbutane->AddElement(tbjcH, 10);

    tbjcHe2 = 
        new G4Material("tbjcHe2", density= 0.00021897*g/cm3, tbjcncomponents=1);
    tbjcHe2->AddElement(tbjcHe,2);

    //  cout<<"d1:"<<tbjcHe2->GetDensity()<<endl;
    //  exit(1);

    tbjcH2O = 
        new G4Material("tbjcH2O", density= 0.000756182*g/cm3, tbjcncomponents=2);
    tbjcH2O->AddElement(tbjcH, 2);
    tbjcH2O->AddElement(tbjcO, 1);


    ////////////////calculate the CF4 density/////////////

    double tbjctorr = 133.322;
    double tbjcPressure = 0* tbjctorr;
    double tbjcTemperature = 0;
    double tbjcRconstant =8.31;


    TBJCINTERFACE::interface->GetAttributes("IonChamberGasP",(void*)&tbjcPressure);
    TBJCINTERFACE::interface->GetAttributes("IonChamberGasT",(void*)&tbjcTemperature); 



    double tbjcCF4moleMass = (12+19*4)*0.001;
    double tbjcCF4density = tbjcPressure*tbjcCF4moleMass/(tbjcRconstant*tbjcTemperature)/1000;


    tbjcCF4 =
        new G4Material("tbjcCF4", density= tbjcCF4density *g/cm3*1.0, tbjcncomponents=2 );
    tbjcCF4->AddElement(tbjcC, 1);
    tbjcCF4->AddElement(tbjcF, 4);


    cout<<"CF4 density:"<<tbjcCF4density<<endl;
    //////////////////////////////////////////////////////



    tbjcVacuum = 
        new G4Material("tbjcVacuum", density= 0*g/cm3, tbjcncomponents=2);
    tbjcVacuum->AddElement(tbjcH, 2);
    tbjcVacuum->AddElement(tbjcO, 1);

    tbjcCD2 = 
        new G4Material("tbjcCD2", density= 0.931781*g/cm3, tbjcncomponents=2);
    tbjcCD2->AddElement(tbjcD, 2);
    tbjcCD2->AddElement(tbjcC, 1);



    G4NistManager* man = G4NistManager::Instance();
    tbjcSi = man->FindOrBuildMaterial("G4_Si");
    tbjcAl = man->FindOrBuildMaterial("G4_Al");
    tbjcFe = man->FindOrBuildMaterial("G4_Fe");
    tbjcCu = man->FindOrBuildMaterial("G4_Cu");

    tbjcKapton = man->FindOrBuildMaterial("G4_KAPTON");




    G4bool checkOverlaps = true;

    ////World////////////////////////
    G4double world_sizeXY = 1.2*180*cm;
    G4double world_sizeZ  = 1.2*400*cm;

    G4Box* solidWorld =    
        new G4Box("World",                       //its name
                0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

    G4LogicalVolume* logicWorld =                         
        new G4LogicalVolume(solidWorld,          //its solid
                world_mat,           //its material
                "World");            //its name

    G4VPhysicalVolume* physWorld = 
        new G4PVPlacement(0,                     //no rotation
                G4ThreeVector(),       //at (0,0,0)
                logicWorld,            //its logical volume
                "World",               //its name
                0,                     //its mother  volume
                false,                 //no boolean operation
                0,                     //copy number
                checkOverlaps);        //overlaps checking

    G4VisAttributes* worldatt=new G4VisAttributes(1,G4Colour::White() );
    worldatt->SetColor(1,0,0,0.1);
    logicWorld->SetVisAttributes(worldatt);
    ////HELIOS inside////////     
    G4ThreeVector pos = G4ThreeVector(0*cm, 0*cm, 0*cm);

    G4EllipticalTube * HELIOSsolidShape =    
        new G4EllipticalTube("HELIOS",HELIOSradius,HELIOSradius,HELIOSlength/2.0);


    G4LogicalVolume* HELIOSlogicShape =                         
        new G4LogicalVolume(HELIOSsolidShape ,         //its solid
                tbjcVacuum,
                "HELIOS");           //its name


    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            HELIOSlogicShape,             //its logical volume
            "HELIOS",                //its name
            //logicEnv,
            logicWorld,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking


    G4VisAttributes* HELIOSatt=new G4VisAttributes(1,G4Colour::White() );
    HELIOSatt->SetColor(1,1,1,0.1);
    HELIOSlogicShape->SetVisAttributes(HELIOSatt);


    constructSTinHELIOS(HELIOSlogicShape);
    constructTarget(HELIOSlogicShape);

    constructSiliconArray(HELIOSlogicShape);
    constructHELIOSshell(logicWorld);
    constructIonFront(logicWorld);



    //
    //always return the physical World
    //
    //
    //
    //


    ////magnetic field///////
/*
    fMagneticField = new tbjcMagneticField();
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(fMagneticField);
    fFieldMgr->CreateChordFinder(fMagneticField);
    G4bool forceToAllDaughters = true;
    logicWorld->SetFieldManager(fFieldMgr, forceToAllDaughters);
*/

    double magneticfield = 0;
//TBJCINTERFACE::interface->PrintOut();
    TBJCINTERFACE::interface->GetAttributes("MagneticFieldZ",(void*)&magneticfield);
    

       fMagneticField = new tbjcMagneticField();
       G4UniformMagField*tbjcField = new G4UniformMagField(G4ThreeVector(0,0,magneticfield*tesla));

    //   fFieldMgr = G4TransportationManager::GetTransportationManager()
    //             ->GetFieldManager();
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(tbjcField);
    fFieldMgr->CreateChordFinder(tbjcField);
    G4bool forceToAllDaughters = true;
    logicWorld->SetFieldManager(fFieldMgr, forceToAllDaughters);
     
    /*
       fFieldMgr->SetDeltaIntersection(1*mm);
       fFieldMgr->SetDeltaOneStep(1*mm);
       fFieldMgr->GetChordFinder()->SetDeltaChord( 0.1*mm);
       fFieldMgr->SetMinimumEpsilonStep( 1e-5 );
       fFieldMgr->SetMaximumEpsilonStep( 1e-4 );
     */

   G4UserLimits*MyLimits = new  G4UserLimits( 0.1*cm,
                DBL_MAX,
                DBL_MAX,
                0.,
                0. );

     HELIOSlogicShape->SetUserLimits(MyLimits);

    cout<<"consctruct complete"<<endl;
    return physWorld;
}




G4VPhysicalVolume* tbjcDetectorConstruction::constructSTinHELIOS(G4LogicalVolume* logicParent)
{
    G4ThreeVector pos;
    
    int checkOverlaps = true;
    
    double IsBlocker = 0;
    
    TBJCINTERFACE::interface->GetAttributes("IsBlocker",(void*)&IsBlocker);

    if(IsBlocker>0.01)
    {
    double blockerR =2.5*cm/2;// 3/4.0/2.0*2.54*cm;

    TBJCINTERFACE::interface->GetAttributes("BlockerRadius",(void*)&blockerR);

    //   cout<<blockerR/cm<<endl;
    //   exit(1);

    //double blockerR = 3/4.0/2.0*2.54*cm;
    
    double blockerL = 12*cm;

    TBJCINTERFACE::interface->GetAttributes("BlockerLength",(void*)&blockerL);

    double blockerZ = 0*cm;
    TBJCINTERFACE::interface->GetAttributes("BlockerPosition",(void*)&blockerZ);

    pos = G4ThreeVector(0,0,blockerZ+blockerL/2.0);

    G4EllipticalTube * blocker =    
        new G4EllipticalTube("blocker",blockerR,blockerR,blockerL/2);

    G4LogicalVolume * blocker_logical =
        new G4LogicalVolume(blocker,
                tbjcAl,
                "blocker_logical",
                0,0,0);

    G4VPhysicalVolume * blocker_physical = 
        new G4PVPlacement(0,
                pos,
                blocker_logical,
                "blocker_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);

    HELIOSBlocker = blocker_logical;

    }

    double IsRecoilStopper = 0;


    TBJCINTERFACE::interface->GetAttributes("IsRecoilStopper",(void*)&IsRecoilStopper);

    if(IsRecoilStopper>0.01)
    {


        double DownFrameBodyR = 30*cm;
        double DownFrameBodyL = 3*cm;
        double DownFrameBodyZ = 0*cm;

    TBJCINTERFACE::interface->GetAttributes("StopperLength",(void*)&DownFrameBodyL);
    TBJCINTERFACE::interface->GetAttributes("StopperPosition",(void*)&DownFrameBodyZ);
    TBJCINTERFACE::interface->GetAttributes("StopperRadius",(void*)&DownFrameBodyR);
    
        double DownFrameHoleR = 4.5*2.54*cm/2;


        pos = G4ThreeVector(0,0,DownFrameBodyZ-DownFrameBodyL/2.0);


        G4EllipticalTube * DownFrameBody =    
            new G4EllipticalTube("DownFrameBody",DownFrameBodyR, DownFrameBodyR, DownFrameBodyL/2);

        /*
           G4EllipticalTube * DownFrameHole  =    
           new G4EllipticalTube("DownFrameHole",
           DownFrameHoleR,
           DownFrameHoleR,
           DownFrameBodyL/2+0.1*cm);

           G4VSolid* DownFrame = 
           new G4SubtractionSolid("DownFrame", 
           DownFrameBody, 
           DownFrameHole); 
         */
        G4LogicalVolume * DownFrame_logical =
            new G4LogicalVolume(DownFrameBody,
                    tbjcAl,
                    "DownFrame_logical",
                    0,0,0);

        new G4PVPlacement(0,
                pos,
                DownFrame_logical,
                "DownFrame_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);

    }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* tbjcDetectorConstruction::constructTarget(G4LogicalVolume* logicParent)
{

    double targetPosition = -10*cm*0;

    TBJCINTERFACE::interface->GetAttributes("TargetPosition",(void*)&targetPosition);

    G4ThreeVector pos = G4ThreeVector(0,0,targetPosition);

    int  fCheckOverlaps =1;

    G4Box*TargetZone = new G4Box("TargetZone", 15*cm/2, 15*cm/2, 2.0*cm/2);

    G4LogicalVolume* TargetZoneLogic
        = new G4LogicalVolume(
                TargetZone,   //its solid
                tbjcVacuum,      //its material
                "TargetZoneLogic"); //its name

    new G4PVPlacement(
            0,               // no rotation
            pos, // at (0,0,0)
            TargetZoneLogic,         // its logical volume
            "TargetZoneLogic",         // its name
            logicParent,               // its mother  volume
            false,           // no boolean operations
            0,               // copy number
            fCheckOverlaps); // checking overlaps 




   G4UserLimits*MyLimits = new  G4UserLimits( 0.1*cm,
                DBL_MAX,
                DBL_MAX,
                0.,
                0. );

    TargetZoneLogic->SetUserLimits(MyLimits);




    G4VisAttributes* att0=new G4VisAttributes(1,G4Colour::White() );
    att0->SetColor(1,0,0,0);
    TargetZoneLogic->SetVisAttributes(att0);

    string TargetType;
    TBJCINTERFACE::interface->GetAttributes("TargetType",(void*)&TargetType);


    string TargetMaterialName;
    TBJCINTERFACE::interface->GetAttributes("TargetMaterial",(void*)&TargetMaterialName);
    G4Material*TargetMaterial =  G4Material::GetMaterial(TargetMaterialName);

    if(strcmp(TargetType.c_str(),"GasTarget")==0)
    {
        ///////////////////GasTarget////////////


        G4RotationMatrix* SphereRotation1 = new G4RotationMatrix();
        SphereRotation1->set(G4ThreeVector(1,0,0),180*degree);


        G4RotationMatrix* SphereRotation2 = new G4RotationMatrix();
        SphereRotation2->set(G4ThreeVector(1,0,0),180*degree);
        //////////////////////////////////////



        const double inch =2.54*cm;
        double tbjcwindowthickness =0;// 7*um;
        double tbjcholeR = 0;//0.25*inch/2;
        double tbjcFlangeWidth =0;// 1*mm;
        double tbjcFlangeRadius =0;// 5*cm;
        double tbjcFlangeThickness =0;// tbjcFlangeWidth*1.2;
        double tbjcballoutfactor =0;//0.4;



        TBJCINTERFACE::interface->GetAttributes("GasTargetFlangeRadius",(void*)&tbjcFlangeRadius);
        TBJCINTERFACE::interface->GetAttributes("GasTargetBallOutFactor",(void*)&tbjcballoutfactor);
        TBJCINTERFACE::interface->GetAttributes("GasTargetFlangeWidth",(void*)&tbjcFlangeWidth);
        TBJCINTERFACE::interface->GetAttributes("GasTargetMetalThickness",(void*)&tbjcFlangeThickness);
        TBJCINTERFACE::interface->GetAttributes("GasTargetHoleRadius",(void*)&tbjcholeR);
        TBJCINTERFACE::interface->GetAttributes("GasTargetWindowThickness",(void*)&tbjcwindowthickness);


        double tbjcballout = tbjcwindowthickness*(1-tbjcballoutfactor)+tbjcholeR*tbjcballoutfactor ; //from tbjcwindowthickness to tbjcholeR

        //TBJCINTERFACE::interface->PrintOut();


        G4EllipticalTube*FlangeTubeOutside = new G4EllipticalTube("FlangeTubeOutside", tbjcholeR*5, tbjcholeR*5, tbjcFlangeWidth/2);

        double tbjcOuterR =(tbjcballout*tbjcballout+tbjcholeR*tbjcholeR)/(2*tbjcballout);
        double tbjcOuterDelta = tbjcballout;

        G4Ellipsoid* balloutshapeoutside = new G4Ellipsoid("balloutshapeoutside",
                tbjcOuterR,
                tbjcOuterR,
                tbjcOuterR,
                tbjcOuterR-tbjcballout,
                tbjcOuterR);



        G4UnionSolid* totalshapeoutside1 = new G4UnionSolid("totalshapeoutside1",FlangeTubeOutside, balloutshapeoutside,0,G4ThreeVector(0,0,tbjcFlangeWidth/2.0-(tbjcOuterR-tbjcOuterDelta)));
        G4UnionSolid* totalshapeoutside2 = new G4UnionSolid("totalshapeoutside2",totalshapeoutside1, balloutshapeoutside,SphereRotation1,G4ThreeVector(0,0,-(tbjcFlangeWidth/2.0-(tbjcOuterR-tbjcOuterDelta))));


        G4LogicalVolume* totalshapeoutsideLogic
            = new G4LogicalVolume(
                    totalshapeoutside2,   //its solid
                    tbjcKapton,      //its material
                    "totalshapeoutsideLogic"); //its name


        new G4PVPlacement(
                0,               // no rotation
                G4ThreeVector(0,0,0*cm), // at (0,0,0)
                totalshapeoutsideLogic,         // its logical volume
                "totalshapeoutsidePVP",         // its name
                TargetZoneLogic,               // its mother  volume
                false,           // no boolean operations
                0,               // copy number
                fCheckOverlaps); // checking overlaps 







        G4VisAttributes* att1=new G4VisAttributes(1,G4Colour::White() );
        att1->SetColor(1,0,0,0.2);
        totalshapeoutsideLogic->SetVisAttributes(att1);


        double tbjcInsideR =(tbjcballout*tbjcballout+tbjcholeR*tbjcholeR)/(2*tbjcballout)-tbjcwindowthickness;
        double tbjcInsideDelta = tbjcballout - tbjcwindowthickness;


        G4EllipticalTube*FlangeTubeInside = new G4EllipticalTube("FlangeTubeInside", tbjcholeR-tbjcwindowthickness, tbjcholeR-tbjcwindowthickness, tbjcFlangeWidth/2);



        G4Ellipsoid* balloutshapeinside = new G4Ellipsoid("balloutshapeinside",
                tbjcInsideR,
                tbjcInsideR,
                tbjcInsideR,
                tbjcInsideR-tbjcInsideDelta,
                tbjcInsideR);



        G4UnionSolid* totalshapeinside1 = new G4UnionSolid("totalshapeinside1",FlangeTubeInside, balloutshapeinside,0,G4ThreeVector(0,0, tbjcFlangeWidth/2.0-(tbjcInsideR-tbjcInsideDelta)   ));
        G4UnionSolid* totalshapeinside2 = new G4UnionSolid("totalshapeinside2",totalshapeinside1, balloutshapeinside,SphereRotation1,G4ThreeVector(0,0,  -(tbjcFlangeWidth/2.0-(tbjcInsideR-tbjcInsideDelta))     ));



        G4LogicalVolume* totalshapeinsideLogic
            = new G4LogicalVolume(
                    totalshapeinside2,   //its solid
                    TargetMaterial,      //its material
                    "totalshapeinsideLogic"); //its name



        new G4PVPlacement(
                0,               // no rotation
                G4ThreeVector(0,0,0*cm), // at (0,0,0)
                totalshapeinsideLogic,         // its logical volume
                "totalshapeinsidePVP",         // its name
                totalshapeoutsideLogic,               // its mother  volume
                false,           // no boolean operations
                0,               // copy number
                fCheckOverlaps); // checking overlaps 


        G4VisAttributes* att2=new G4VisAttributes(1,G4Colour::White() );
        att2->SetColor(1,1,0,1);
        totalshapeinsideLogic->SetVisAttributes(att2);




        G4Tubs* GasTargetFlange= new G4Tubs("GasTargetFlange", tbjcholeR, 
                tbjcFlangeRadius,tbjcFlangeThickness/2,0*degree,360*degree); 

        G4LogicalVolume* GasTargetFlangeLogic
            = new G4LogicalVolume(
                    GasTargetFlange,   //its solid
                    tbjcCu,      //its material
                    "GasTargetFlangeLogic"); //its name



        new G4PVPlacement(
                0,               // no rotation
                G4ThreeVector(0,0,(tbjcFlangeThickness+tbjcFlangeWidth)/2), // at (0,0,0)
                GasTargetFlangeLogic,         // its logical volume
                "GasTargetFlangePVP",         // its name
                TargetZoneLogic,               // its mother  volume
                false,           // no boolean operations
                0,               // copy number
                fCheckOverlaps); // checking overlaps 

        new G4PVPlacement(
                0,               // no rotation
                G4ThreeVector(0,0,-(tbjcFlangeThickness+tbjcFlangeWidth)/2), // at (0,0,0)
                GasTargetFlangeLogic,         // its logical volume
                "GasTargetFlangePVP",         // its name
                TargetZoneLogic,               // its mother  volume
                false,           // no boolean operations
                1,               // copy number
                fCheckOverlaps); // checking overlaps 

        G4VisAttributes* att4=new G4VisAttributes(1,G4Colour::White() );
        att4->SetColor(0.58,0.54,0.05,1);
        GasTargetFlangeLogic->SetVisAttributes(att4);

        ballouttest = totalshapeoutsideLogic;
        ballouttest2 = totalshapeinsideLogic;


        TargetVolume = totalshapeinsideLogic;
    }



    if(strcmp(TargetType.c_str(),"SolidTarget")==0)
    {

        int checkOverlaps = 1;
        G4ThreeVector pos = G4ThreeVector(0,0,0);


        // double SolidThickness = 4.03783e-4*cm;
        double SolidThickness = 2e-4*cm;
        double SolidR = 0.6*cm;

        TBJCINTERFACE::interface->GetAttributes("SolidTargetRadius",(void*)&SolidR);
        TBJCINTERFACE::interface->GetAttributes("SolidTargetThickness",(void*)&SolidThickness);




        G4EllipticalTube * targetSolid =    
            new G4EllipticalTube("targetSolid",SolidR,SolidR,SolidThickness/2);

        G4LogicalVolume * targetSolid_logical =
            new G4LogicalVolume(targetSolid,
                    TargetMaterial,
                    "targetSolid_logical",
                    0,0,0);

           //     cout<<SolidThickness/cm<<" "<<TargetMaterial->GetDensity()/(g/(cm*cm*cm))<<endl;
          //      exit(1);

        G4VPhysicalVolume * targetSolid_physical = 
            new G4PVPlacement(0,
                    pos,
                    targetSolid_logical,
                    "targetSolid_physical",
                    TargetZoneLogic, 
                    false,
                    0,
                    checkOverlaps);

        G4VisAttributes* targetSolidatt=new G4VisAttributes(1,G4Colour::White() );
        targetSolidatt->SetColor(1,1,0,0.5);
        targetSolid_logical->SetVisAttributes(targetSolidatt);

        TargetVolume = targetSolid_logical;

        //        if(TargetVolume==0)cout<<"crap"<<endl;
        //    exit(1);
    }
/*
    double blockerR =2.5*cm/2;// 3/4.0/2.0*2.54*cm;

    TBJCINTERFACE::interface->GetAttributes("BlockerRadius",(void*)&blockerR);

    //   cout<<blockerR/cm<<endl;
    //   exit(1);

    //double blockerR = 3/4.0/2.0*2.54*cm;
    
    double blockerL = 12*cm;

    TBJCINTERFACE::interface->GetAttributes("BlockerLength",(void*)&blockerL);

    double blockerZ = 0*cm;
    TBJCINTERFACE::interface->GetAttributes("BlockerPosition",(void*)&blockerZ);

    pos = G4ThreeVector(0,0,blockerZ+blockerL/2.0);

    G4EllipticalTube * blocker =    
        new G4EllipticalTube("blocker",blockerR,blockerR,blockerL/2);

    G4LogicalVolume * blocker_logical =
        new G4LogicalVolume(blocker,
                tbjcAl,
                "blocker_logical",
                0,0,0);

    G4VPhysicalVolume * blocker_physical = 
        new G4PVPlacement(0,
                pos,
                blocker_logical,
                "blocker_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);

    HELIOSBlocker = blocker_logical;
    */
        return NULL;
}




G4VPhysicalVolume* tbjcDetectorConstruction::constructIonFront(G4LogicalVolume* logicParent)
{


    G4bool checkOverlaps = true;
    double IONStart = HELIOSlength/2+3.375*cm+HELIOSshellThickness+0.1*cm;

    ////ion front//////////
    G4ThreeVector offset = G4ThreeVector(0,0,0);
    CADMesh * ionfront = new CADMesh("ion_shell_front.stl","STL",cm,offset,false);
    G4VSolid * ionfront_solid = ionfront->TessellatedMesh();

    G4LogicalVolume * ionfront_logical = new G4LogicalVolume(ionfront_solid, tbjcAl, "ionfront_logical",0,0,0);

    G4VPhysicalVolume * ionfront_physical = 
        new G4PVPlacement(0,
                G4ThreeVector(0,0,IONStart),
                ionfront_logical,
                "ionfront_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);

    G4VisAttributes* IONfrontatt=new G4VisAttributes(1,G4Colour::White() );
    IONfrontatt->SetColor(1,1,1,1);
    ionfront_logical->SetVisAttributes(IONfrontatt);


    /////rods in the ion////

    G4ThreeVector pos = G4ThreeVector(0,0,0);
    /*
       double rodsR = 0.2502*cm;
       double rodsPositionR = 10.16*cm;
       double rodsLength = 63.03*cm;

       G4ThreeVector pos = G4ThreeVector(0*cm, rodsPositionR, HELIOSlength/2+3.375*cm+rodsLength/2+3*cm-1*cm+0.5*cm);
    // Trapezoid shape       

    G4EllipticalTube * ionrods =    
    new G4EllipticalTube("ionrods",rodsR,rodsR,rodsLength/2.0);


    G4LogicalVolume* rodslogicshape =                         
    new G4LogicalVolume(ionrods ,         //its solid
    tbjcFe,
    "ionrods_logic");           //its name

    for(int i=0;i<4;i++)
    {
    pos.setX(rodsPositionR*sin(90*degree*i));
    pos.setY(rodsPositionR*cos(90*degree*i));
    new G4PVPlacement(0,                       //no rotation
    pos,                    //at position
    rodslogicshape,             //its logical volume
    "ionrods_physics",                //its name
    //logicEnv,
    logicParent,              //its mother  volume
    false,                   //no boolean operation
    i,                       //copy number
    checkOverlaps);          //overlaps checking
    }
     */

    /////ion shell////////

    double IONinnerR = 22.8346*cm/2+0.03*cm;
    double IONouterR = 23.5204*cm/2-0.03*cm;
    double IONLength = 66.03*cm+1*cm;

    pos.setX(0);pos.setY(0);pos.setZ(IONStart+3*cm-1*cm+IONLength/2);
    G4EllipticalTube * IONinner =    
        new G4EllipticalTube("IONinner",IONinnerR,IONinnerR,IONLength/2);

    G4EllipticalTube * IONouter =    
        new G4EllipticalTube("IONouter",
                IONouterR,
                IONouterR,
                IONLength/2);

    G4VSolid* IONshell = 
        new G4SubtractionSolid("IONshell", 
                IONouter, 
                IONinner); 


    G4LogicalVolume * IONshell_logical =
        new G4LogicalVolume(IONshell,
                tbjcAl,
                "IONshell_logical",
                0,0,0);

    G4VPhysicalVolume * IONshell_physical = 
        new G4PVPlacement(0,
                pos,
                IONshell_logical,
                "IONshell_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);


    G4VisAttributes* IONshellatt=new G4VisAttributes(1,G4Colour::White() );
    IONshellatt->SetColor(1,1,1,0);
    IONshell_logical->SetVisAttributes(IONshellatt);



    string IonChamberMaterialName;
    TBJCINTERFACE::interface->GetAttributes("IonChamberGasType",(void*)&IonChamberMaterialName);
    G4Material*IonChamberMaterial =  G4Material::GetMaterial(IonChamberMaterialName);





    /////ion start inside space//////////
    double IONstartInsideR = 16.5*cm/2-0.05*cm;
    double IONstartInsideL = 1.7*cm*2; 


    pos.setX(0);pos.setY(0);pos.setZ(IONStart+3*cm-IONstartInsideL/2);
    G4EllipticalTube * IONstartInside =    
        new G4EllipticalTube("IONstartInside",IONstartInsideR,IONstartInsideR,IONstartInsideL/2);

    G4LogicalVolume * IONstartInside_logical =
        new G4LogicalVolume(IONstartInside,
                IonChamberMaterial,
                "IONstartInside_logical",
                0,0,0);

    G4VPhysicalVolume * IONstartInside_physical = 
        new G4PVPlacement(0,
                pos,
                IONstartInside_logical,
                "IONstartInside_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);


    G4VisAttributes* IONstartInsideatt=new G4VisAttributes(1,G4Colour::White() );
    IONstartInsideatt->SetColor(0,1,0,0.2);
    IONstartInside_logical->SetVisAttributes(IONstartInsideatt);


    //////ion window///////////////
    double windowThickness =  7.0422535*um;


    pos.setX(0);pos.setY(0);pos.setZ(IONStart+3*cm-IONstartInsideL-windowThickness/2);

    G4EllipticalTube * IONwindow =    
        new G4EllipticalTube("IONwindow",IONstartInsideR,IONstartInsideR, windowThickness/2);

    G4LogicalVolume * IONwindow_logical =
        new G4LogicalVolume(IONwindow,
                tbjcKapton,
                "IONwindow_logical",
                0,0,0);

    G4VPhysicalVolume * IONwindow_physical = 
        new G4PVPlacement(0,
                pos,
                IONwindow_logical,
                "IONwindow_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);


    G4VisAttributes* IONwindowatt=new G4VisAttributes(1,G4Colour::White() );
    IONwindowatt->SetColor(0.2,1,0.3,1);
    IONwindow_logical->SetVisAttributes(IONwindowatt);






    ////ion inner space/////////////////

    double IONinsideR = IONinnerR;
    double IONinsideLength = IONLength;

    pos.setX(0);pos.setY(0);pos.setZ(IONStart+3*cm+IONinsideLength/2);

    G4EllipticalTube * IONinside =    
        new G4EllipticalTube("IONinside",IONinsideR,IONinsideR,IONinsideLength/2);

    G4LogicalVolume * IONinside_logical =
        new G4LogicalVolume(IONinside,
                tbjcVacuum,
                "IONinside_logical",
                0,0,0);

    G4VPhysicalVolume * IONinside_physical = 
        new G4PVPlacement(0,
                pos,
                IONinside_logical,
                "IONinside_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);

    G4VisAttributes* IONinsideatt=new G4VisAttributes(1,G4Colour::White() );
    IONinsideatt->SetColor(0,0,1,0.2);
    IONinside_logical->SetVisAttributes(IONinsideatt);


    //// everything inside ion chamber////////////
    constructIonInside(IONinside_logical);


    return NULL;
}

G4VPhysicalVolume * tbjcDetectorConstruction::constructIonInside(G4LogicalVolume* logicParent)
{

    G4bool checkOverlaps = true;
    double IONinsideL = ((G4EllipticalTube*)logicParent->GetSolid())->GetDz()*2;
    double IONinsideR = ((G4EllipticalTube*)logicParent->GetSolid())->GetDx();
    double startpoint = -IONinsideL/2;

    G4ThreeVector pos;
    /////rods in the ion////
    /*
       double rodsR = 0.2502*cm;
       double rodsPositionR = 10.16*cm;
       double rodsLength = IONinsideL;

       G4ThreeVector pos = G4ThreeVector(0*cm, rodsPositionR,rodsLength/2 - IONinsideL/2);

       G4EllipticalTube * ionrods =    
       new G4EllipticalTube("ionrods",rodsR,rodsR,rodsLength/2.0);


       G4LogicalVolume* rodslogicshape =                         
       new G4LogicalVolume(ionrods ,         //its solid
       tbjcFe,
       "ionrods_logic");           //its name

       for(int i=0;i<4;i++)
       {
       pos.setX(rodsPositionR*sin(90*degree*i));
       pos.setY(rodsPositionR*cos(90*degree*i));
       new G4PVPlacement(0,                       //no rotation
       pos,                    //at position
       rodslogicshape,             //its logical volume
       "ionrods_physics",                //its name
    //logicEnv,
    logicParent,              //its mother  volume
    false,                   //no boolean operation
    i,                       //copy number
    checkOverlaps);          //overlaps checking
    }

    G4VisAttributes* rodsatt=new G4VisAttributes(1,G4Colour::White() );
    rodsatt->SetColor(0,1,1,0.5);
    rodslogicshape->SetVisAttributes(rodsatt);

     */

    ////////regular grids//////////////////////

    ////I am just going to leave three sections here//// 


    string IonChamberMaterialName;
    TBJCINTERFACE::interface->GetAttributes("IonChamberGasType",(void*)&IonChamberMaterialName);
    G4Material*IonChamberMaterial =  G4Material::GetMaterial(IonChamberMaterialName);



    double DEADSPACElen= 1.6688*cm+0*cm;
    pos = G4ThreeVector(0*cm, 0,DEADSPACElen/2+startpoint);

    G4EllipticalTube * DEADSPACE =    
        new G4EllipticalTube("DEADSPACE",IONinsideR,IONinsideR,DEADSPACElen/2);


    G4LogicalVolume* DEADSPACElogic =                         
        new G4LogicalVolume(DEADSPACE ,         //its solid
                IonChamberMaterial,
                "DEADSPACElogic");           //its name

    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            DEADSPACElogic,             //its logical volume
            "DEADSPACE",                //its name
            //logicEnv,
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* DEADSPACEatt=new G4VisAttributes(1,G4Colour::White() );
    DEADSPACEatt->SetColor(1,0,1,0.5);
    DEADSPACElogic->SetVisAttributes(DEADSPACEatt);


    /////////////////////////////////////


    double Sec0_len= 4*cm;
    pos = G4ThreeVector(0*cm, 0,DEADSPACElen+startpoint+Sec0_len/2);

    G4EllipticalTube * Sec0 =    
        new G4EllipticalTube("Sec0",IONinsideR,IONinsideR,Sec0_len/2);


    G4LogicalVolume* Sec0logic =                         
        new G4LogicalVolume(Sec0 ,         //its solid
                IonChamberMaterial,
                "Sec0logic");           //its name

    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            Sec0logic,             //its logical volume
            "Sec0",                //its name
            //logicEnv,
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* Sec0att=new G4VisAttributes(1,G4Colour::White() );
    Sec0att->SetColor(1,0,0,0.5);
    Sec0logic->SetVisAttributes(Sec0att);


    /////////////////////////////////////


    double Sec1_len= 8*cm;
    pos = G4ThreeVector(0*cm, 0,DEADSPACElen+startpoint+Sec0_len+Sec1_len/2);

    G4EllipticalTube * Sec1 =    
        new G4EllipticalTube("Sec1",IONinsideR,IONinsideR,Sec1_len/2);


    G4LogicalVolume* Sec1logic =                         
        new G4LogicalVolume(Sec1 ,         //its solid
                IonChamberMaterial,
                "Sec1logic");           //its name

    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            Sec1logic,             //its logical volume
            "Sec1",                //its name
            //logicEnv,
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* Sec1att=new G4VisAttributes(1,G4Colour::White() );
    Sec1att->SetColor(1,1,0,0.5);
    Sec1logic->SetVisAttributes(Sec1att);


    ///////////////////////////////////////

    double Sec2_len= 12*cm;
    pos = G4ThreeVector(0*cm, 0,DEADSPACElen+startpoint+Sec0_len+Sec1_len+Sec2_len/2);

    G4EllipticalTube * Sec2 =    
        new G4EllipticalTube("Sec2",IONinsideR,IONinsideR,Sec2_len/2);


    G4LogicalVolume* Sec2logic =                         
        new G4LogicalVolume(Sec2 ,         //its solid
                IonChamberMaterial,
                "Sec2logic");           //its name

    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            Sec2logic,             //its logical volume
            "Sec2",                //its name
            //logicEnv,
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* Sec2att=new G4VisAttributes(1,G4Colour::White() );
    Sec2att->SetColor(0,1,0,0.5);
    Sec2logic->SetVisAttributes(Sec2att);

    ///////////////////////////////////////

    double Sec3_len= 16*cm;
    pos = G4ThreeVector(0*cm, 0,DEADSPACElen+startpoint+Sec0_len+Sec1_len+Sec2_len+Sec3_len/2);

    G4EllipticalTube * Sec3 =    
        new G4EllipticalTube("Sec3",IONinsideR,IONinsideR,Sec3_len/2);


    G4LogicalVolume* Sec3logic =                         
        new G4LogicalVolume(Sec3,         //its solid
                IonChamberMaterial,
                "Sec3logic");           //its name

    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            Sec3logic,             //its logical volume
            "Sec3",                //its name
            //logicEnv,
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* Sec3att=new G4VisAttributes(1,G4Colour::White() );
    Sec3att->SetColor(0,0,1,0.5);
    Sec3logic->SetVisAttributes(Sec3att);



    ///////////////////////////////////////

    double Sec4_len= 12*cm;
    pos = G4ThreeVector(0*cm, 0,DEADSPACElen+startpoint+Sec0_len+Sec1_len+Sec2_len+Sec3_len+Sec4_len/2);

    G4EllipticalTube * Sec4 =    
        new G4EllipticalTube("Sec4",IONinsideR,IONinsideR,Sec4_len/2);


    G4LogicalVolume* Sec4logic =                         
        new G4LogicalVolume(Sec4,         //its solid
                IonChamberMaterial,
                "Sec4logic");           //its name

    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            Sec4logic,             //its logical volume
            "Sec4",                //its name
            //logicEnv,
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* Sec4att=new G4VisAttributes(1,G4Colour::White() );
    Sec4att->SetColor(1,0,1,0.5);
    Sec4logic->SetVisAttributes(Sec4att);

    ////////////////////////////////////////////////////////////////////////////
    double Sec5_len= 3*cm;
    pos = G4ThreeVector(0*cm, 0,DEADSPACElen+startpoint+Sec0_len+Sec1_len+Sec2_len+Sec3_len+Sec4_len+Sec5_len/2);

    G4EllipticalTube * Sec5 =    
        new G4EllipticalTube("Sec5",IONinsideR,IONinsideR,Sec5_len/2);


    G4LogicalVolume* Sec5logic =                         
        new G4LogicalVolume(Sec5,         //its solid
                tbjcAl,
                "Sec5logic");           //its name

    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            Sec5logic,             //its logical volume
            "Sec5",                //its name
            //logicEnv,
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* Sec5att=new G4VisAttributes(1,G4Colour::White() );
    Sec5att->SetColor(0,1,1,0.5);
    Sec5logic->SetVisAttributes(Sec5att);





    Ionsections[0] = Sec0logic;
    Ionsections[1] = Sec1logic;
    Ionsections[2] = Sec2logic;
    Ionsections[3] = Sec3logic;
    Ionsections[4] = Sec4logic;
    Ionsections[5] = Sec5logic;

    return NULL;
}


G4VPhysicalVolume* tbjcDetectorConstruction::constructHELIOSshell(G4LogicalVolume* logicParent)
{

    ///////HELIOS back and fromt////////////// 
    G4bool checkOverlaps = true;

    G4EllipticalTube * HELIOSbackouter =    
        new G4EllipticalTube("HELIOSbackouter",
                HELIOSradius+HELIOSshellThickness,
                HELIOSradius+HELIOSshellThickness,
                HELIOSshellThickness/2);

    double HELIOSbackinnerD = 72*cm;
    /*
       G4EllipticalTube * HELIOSbackinner =    
       new G4EllipticalTube("HELIOSbackinner",HELIOSbackinnerD/2,HELIOSbackinnerD/2,HELIOSshellThickness/2+0.5*cm);


       G4VSolid* HELIOSback = 
       new G4SubtractionSolid("HELIOSback", 
       HELIOSbackouter, 
       HELIOSbackinner); 
     */

    G4Tubs* HELIOSback = new G4Tubs("HELIOSback",  HELIOSbackinnerD/2, HELIOSradius+HELIOSshellThickness, HELIOSshellThickness/2, 0*degree, 360*degree);



    G4LogicalVolume * HELIOSback_logical =
        new G4LogicalVolume(HELIOSback,
                tbjcAl,
                "HELIOSback_logical",
                0,0,0);

    G4VPhysicalVolume * HELIOSback_physical = 
        new G4PVPlacement(0,
                G4ThreeVector(0,0,HELIOSlength/2+HELIOSshellThickness/2),
                HELIOSback_logical,
                "HELIOSback_physical",
                logicParent, 
                false,
                0,
                checkOverlaps);

    new G4PVPlacement(0,
            G4ThreeVector(0,0,-(HELIOSlength/2+HELIOSshellThickness/2)),
            HELIOSback_logical,
            "HELIOSfront_physical",
            logicParent, 
            false,
            0,
            checkOverlaps);


    ///////HELIOS shell/////////////



    G4Tubs* HELIOSshell = new G4Tubs("HELIOSshell",  HELIOSradius, HELIOSradius+HELIOSshellThickness, HELIOSlength/2, 0*degree, 360*degree);

    G4LogicalVolume * HELIOSshell_logical =
        new G4LogicalVolume(HELIOSshell,
                tbjcAl,
                "HELIOSshell_logical",
                0,0,0);

    G4VPhysicalVolume * HELIOSshell_physical = 
        new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                HELIOSshell_logical,
                "HELIOSshell_physical",
                logicParent, 
                false, 
                0,
                checkOverlaps);



    ////////HELIOS door//////////////

    G4ThreeVector offset = G4ThreeVector(0,0,0);
    CADMesh * heliosdoor = new CADMesh("doorfff.stl","STL",cm,offset,false);
    G4VSolid * heliosdoor_solid = heliosdoor->TessellatedMesh();

    G4LogicalVolume * heliosdoor_logical =
        new G4LogicalVolume(heliosdoor_solid,
                tbjcAl,
                "heliosdoor_logical",
                0,0,0);

    G4VPhysicalVolume * heliosdoor_physical = 
        new G4PVPlacement(0,
                G4ThreeVector(0,0,HELIOSlength/2+HELIOSshellThickness),
                heliosdoor_logical,
                "heliosdoor_physical",
                logicParent, 
                false, 
                0,
                checkOverlaps);




    return NULL;
}


G4VPhysicalVolume* tbjcDetectorConstruction::constructSiliconArray(G4LogicalVolume* logicParent)                         
{


    G4bool checkOverlaps = true;

    double ArrayEnd =-20*cm;

    float ContainerLength = 53*cm;
    float ContainerWidth = 60*cm;

    TBJCINTERFACE::interface->GetAttributes("ArrayPosition",(void*)&ArrayEnd);
    double IsRotatedArray = 0;
    TBJCINTERFACE::interface->GetAttributes("IsArrayRotated",(void*)&IsRotatedArray);
    //TBJCINTERFACE::interface->PrintOut();


    G4ThreeVector ArrayRotationAxis = G4ThreeVector(0*cm, 1*cm, 0*cm);
    G4RotationMatrix* ArrayRotationtmp = new G4RotationMatrix();
    ArrayRotationtmp->set(ArrayRotationAxis,180*degree*IsRotatedArray);



    G4ThreeVector pos = G4ThreeVector(0*cm, -20*cm, 0*cm);
    // Trapezoid shape       

    G4Box* ArrayContainerSolidShape =    
        new G4Box("ArrayContainer",                       //its name
                ContainerWidth/2.0, ContainerWidth/2.0, ContainerLength/2.0);     //its size

    G4LogicalVolume*  ArrayContainerlogicShape =                         
        new G4LogicalVolume(ArrayContainerSolidShape,         //its solid
                tbjcVacuum,
                "ArrayContainer");           //its name


    pos.setX(0*cm);pos.setY(0*cm);pos.setZ(-ContainerLength/2.0+ContainerLength*IsRotatedArray+ArrayEnd);
    new G4PVPlacement( ArrayRotationtmp,                       //no rotation
            pos,                    //at position
            ArrayContainerlogicShape,             //its logical volume
            "ArrayContainer",                //its name
            logicParent,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking


   G4UserLimits*MyLimits = new  G4UserLimits( 0.1*cm,
                DBL_MAX,
                DBL_MAX,
                0.,
                0. );

    ArrayContainerlogicShape->SetUserLimits(MyLimits);




    G4VisAttributes* Containeratt=new G4VisAttributes(1,G4Colour::White());
    Containeratt->SetColor(0,1,0,0.1);
    ArrayContainerlogicShape->SetVisAttributes(Containeratt);

    /////////////////////////////////////////////////////////////////////

    float SupportWidth = 21.6*mm;
    float SupportLength = 500*mm;
    float SupportHoleR =0.5*cm;

    G4Box* ArraySupportOutSolidShape =    
        new G4Box("ArrayContainerOut",                       //its name
                SupportWidth/2.0, SupportWidth/2.0, SupportLength/2.0);     //its size
/*

    G4EllipticalTube* ArraySupportOutSolidShape =    
        new G4EllipticalTube("ArraySupportInner", SupportWidth/2.0, SupportWidth/2.0, SupportLength/2);
*/



    G4EllipticalTube* ArraySupportInnerSolidShape =    
        new G4EllipticalTube("ArraySupportInner", SupportHoleR, SupportHoleR, SupportLength/2);

    G4VSolid* ArraySupportSolidShape = 
        new G4SubtractionSolid("ArraySupport", 
                ArraySupportOutSolidShape, 
                ArraySupportInnerSolidShape); 


    G4LogicalVolume* ArraySupportlogicShape =
        new G4LogicalVolume(ArraySupportSolidShape,
                tbjcAl,
                "ArraySupport");

    pos.setX(0*cm);pos.setY(0*cm);pos.setZ(0*cm);
    new G4PVPlacement(0,                       //no rotation
            pos,                    //at position
            ArraySupportlogicShape,             //its logical volume
            "ArraySupport",                //its name
            ArrayContainerlogicShape,              //its mother  volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //overlaps checking

    G4VisAttributes* Supportatt=new G4VisAttributes(1,G4Colour::White());
    Supportatt->SetColor(1,1,1,0.6);
    ArraySupportlogicShape->SetVisAttributes(Supportatt);

    ///////////////////////////////////////////////////////////////////////

    double IsArrayShielded = 0;
    TBJCINTERFACE::interface->GetAttributes("IsArrayShielded",(void*)&IsArrayShielded);
    if(IsArrayShielded >0.01)
    {
    float BeamLineShieldR =30*cm;
    float BeamLineShieldThickness = 1*cm;

    G4Tubs* BeamLineShield = new G4Tubs("BeamLineShield",  SupportWidth/2, BeamLineShieldR, BeamLineShieldThickness/2, 0*degree, 360*degree);

    G4LogicalVolume *BeamLineShieldLogic
        = new G4LogicalVolume(
                BeamLineShield,   //its solid
                tbjcAl,      //its material
                "BeamLineShieldLogic"); //its name

    new G4PVPlacement(
            0,               // no rotation
            G4ThreeVector(0,0,-(SupportLength+BeamLineShieldThickness)/2), // at (0,0,0)
            BeamLineShieldLogic,         // its logical volume
            "BeamLineShieldPVP",         // its name
            ArrayContainerlogicShape,               // its mother  volume
            false,           // no boolean operations
            0,               // copy number
            checkOverlaps); // checking overlaps 



    G4VisAttributes* att3=new G4VisAttributes(1,G4Colour::White() );
    att3->SetColor(0.804, 0.804, 0.757,0.5);
    BeamLineShieldLogic->SetVisAttributes(att3);
    }




    ///////////////////////////////////////////////////////////////////////

    float SiThickness = 0.7*mm;
    float SiWidth = 9*mm;
    float SiLength = 50.5*mm;

    G4Box* SiDetectorSolidShape = 
        new G4Box("SiDetector",
                SiWidth/2.0,SiThickness/2.0,SiLength/2.0);


    G4LogicalVolume* SiDetectorlogicShape =
        new G4LogicalVolume(SiDetectorSolidShape,
                tbjcSi,
                "SiDetector");

    float SiDetectorOffset=3*mm;
    float SiDetectorGap=3*mm;

    G4ThreeVector SiRotationAxis = G4ThreeVector(0*cm, 0*cm, 1*cm);
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<6;j++)
        {

            G4RotationMatrix* SiRotationtmp = new G4RotationMatrix();
            SiRotationtmp->set(SiRotationAxis,90*degree*i);
            pos.setX((SupportWidth/2+SiThickness/2)*sin(90*i*degree));
            pos.setY((SupportWidth/2+SiThickness/2)*cos(90*i*degree));
            pos.setZ(ContainerLength/2-SiLength*0.5-(SiLength+SiDetectorGap)*j-SiDetectorOffset);

            //     cout<<"z1:"<<(ContainerLength/2-SiLength*0.5-(SiLength+SiDetectorGap)*j-SiDetectorOffset-SiLength/2-ContainerLength/2.0+ArrayEnd)/cm; 
            //     cout<<";z2:"<<(ContainerLength/2-SiLength*0.5-(SiLength+SiDetectorGap)*j-SiDetectorOffset+SiLength/2-ContainerLength/2.0+ArrayEnd)/cm;
            //     cout<<endl;
           new G4PVPlacement(SiRotationtmp,                       //no rotation
                    pos,                    //at position
                    SiDetectorlogicShape,             //its logical volume
                    "SiDetector",                //its name
                    ArrayContainerlogicShape,              //its mother  volume
                    false,                   //no boolean operation
                    i*6+j,                       //copy number
                    checkOverlaps);          //overlaps checking 
     
       }
    }
    G4VisAttributes* Detectoratt=new G4VisAttributes(1,G4Colour::White());
    Detectoratt->SetColor(1,1,0,0.2);
    SiDetectorlogicShape->SetVisAttributes(Detectoratt);


    //SIdetector = ArraySupportlogicShape; //HELIOSlogicShape;
    SIdetector = SiDetectorlogicShape; //HELIOSlogicShape;

    return NULL;
}






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
