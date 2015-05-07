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
// $Id: tbjcPrimaryGeneratorAction.cc 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file tbjcPrimaryGeneratorAction.cc
/// \brief Implementation of the tbjcPrimaryGeneratorAction class
#include "tbjcDetectorConstruction.hh"
#include "tbjcPrimaryGeneratorAction.hh"
#include "tbjcSteppingAction.hh"
#include "tbjcEventAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4IonTable.hh"
#include "KINECAL.hpp"
#include <iostream>
#include <fstream>
#include "TBJCBEAM.hpp"
#include "Randomize.hh"
#include "BEAMCOMP.hpp"
#include "BEAMCOMPGENERATOR.hpp"
#include "BEAMCOMPINFO.hpp"
#include "ATOMICMASSTABLE.hpp"
#include <vector>
#include "TBJCINTERFACE.hpp"
using namespace std;

//ofstream pp("protons.dat");


tbjcPrimaryGeneratorAction* tbjcPrimaryGeneratorAction:: generator=0;
    tbjcPrimaryGeneratorAction::tbjcPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
    fParticleGun(0)
{

    generator=this;

    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);
    tbjcbeamer = new TBJCBEAM(); //it is not used yet

    startpoint = G4ThreeVector(0,0,-70*cm);


    masstable = new ATOMICMASSTABLE("AtomicMassTable.db","AtomicMassTable");


    /////////////////1(0,2)3/////////////// 

    int A0 = 0, Z0 = 0;
    int A1 = 0, Z1 = 0;
    int A2 = 0, Z2 = 0;
    int A3 = 0, Z3 = 0;
    double K0 = 0;


    
    TBJCINTERFACE*interface = TBJCINTERFACE::interface;



    interface->GetAttributes("A0",(void*)&A0);
    interface->GetAttributes("Z0",(void*)&Z0);
    interface->GetAttributes("A1",(void*)&A1);
    interface->GetAttributes("Z1",(void*)&Z1);
    interface->GetAttributes("A2",(void*)&A2);
    interface->GetAttributes("Z2",(void*)&Z2);
    interface->GetAttributes("A3",(void*)&A3);
    interface->GetAttributes("Z3",(void*)&Z3);
    interface->GetAttributes("BeamEnergy",(void*)&K0);


    vector<pair<double, double>> Ex2Chance;// = {{0,1}, {0.43999,1},{2.076011,1},{2.390732,1},{2.63985,1},{2.703500,1},{2.982060,1},{3.67760,1},{3.848069,1},{3.914239,1},{4.42964,1},{4.77461,1}};

    interface->GetAttributes("Ex2Chance",(void*)&Ex2Chance);

    mass0 = masstable->SelectEntry(A0,Z0);
    mass1 = masstable->SelectEntry(A1,Z1);
    mass2 = masstable->SelectEntry(A2,Z2);
    mass3 = masstable->SelectEntry(A3,Z3);

    kin = new KINECAL();
    kin->setMass(mass0,mass1,mass2,mass3);


    tbjctable= G4IonTable::GetIonTable();

    cyclecounts=0;
    generateType=0;


    tbjcbg =new BEAMCOMPGENERATOR();
    tbjcbg->addComp(Z0,A0,Z0,0,K0,1);

    tbjcrg =new BEAMCOMPGENERATOR();


    for(vector<pair<double, double> >::iterator it = Ex2Chance.begin(); it != Ex2Chance.end(); it++)
    {

   //     cout<<it->first<<":"<<it->second<<endl;
        tbjcrg->addComp(Z2,A2,Z2,it->first,0,it->second);
    }
//exit(1);
    //  tbjcrg->addComp(10,21,10,0,0,0.1);

    /*
       tbjcrg->addComp(10,21,10,0.3788,0,1);

       tbjcrg->addComp(10,21,10,1.745911,0,0.1);
       tbjcrg->addComp(10,21,10,2.79416 ,0,1);
       tbjcrg->addComp(10,21,10,3.73559 ,0,0.3);
       tbjcrg->addComp(10,21,10,4.52586 ,0,0.5);
       tbjcrg->addComp(10,21,10,4.72534 ,0,0.5);
       */
    tbjclg =new BEAMCOMPGENERATOR();
    tbjclg->addComp(Z3,A3,Z3,0,0,1);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcPrimaryGeneratorAction::~tbjcPrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void tbjcPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //this function is called at the begining of ecah event
    //




    cyclecounts++;
    cout<<"cycle "<<cyclecounts<<endl;

    BEAMCOMPINFO pickinfo;


    switch(generateType)
    {

        case 2:
            {
                cout<<2<<endl;
                cout<<"Kr is "<<Kr<<endl;
                kin->setK0(Kr);
                BEAMCOMP* pick= tbjcrg->generateBeam();
                pickinfo.ParticleComp = pick; 
                cout<<"excited energy"<<pick->Ex<<endl;

                kin->setEx2(pick->Ex);
                kin->randomgenerate();

                if(kin->K2>0)
                {
                    pickinfo.K = kin->K2;
                    
                    G4ThreeVector ParticleDirection;
                    ParticleDirection.setX(sin(kin->thetalab2)*cos(kin->philab2));
                    ParticleDirection.setY(sin(kin->thetalab2)*sin(kin->philab2));
                    ParticleDirection.setZ(cos(kin->thetalab2));
                    pickinfo.ParticleDirection = ParticleDirection;

                    pickinfo.ParticlePosition = startpoint;
                   // pickinfo.ParticlePosition = G4ThreeVector(0,0,-10*cm);
                    
                    generateP0(&pickinfo);

                    tbjcEventAction::EventAction->updateBeamType(generateType);
                    tbjcEventAction::EventAction->updateCMSangle(kin->thetaCMS, kin->phiCMS);

                    generateType--;
                    cout<<"I am here"<<endl;

                    break;
                }
                else
                {
                    cout<<"reset generate type"<<endl; 
                    generateType=0;
                }

            }



        case 0:
            {

                cout<<0<<endl;
                tbjcSteppingAction::STaction->zerotbjc();
                pickinfo.ParticleComp = tbjcbg->generateBeam(); 
                pickinfo.K = pickinfo.ParticleComp->K;

                double RR= G4UniformRand()*1*mm;
                double pp= G4UniformRand()*360*degree;
                double xx= RR*cos(pp);
                double yy= RR*sin(pp);

                pickinfo.ParticlePosition = G4ThreeVector(xx,yy,-100*cm);
            //    pickinfo.ParticlePosition = G4ThreeVector(0,0,-100*cm);

                double costheta =1 - G4UniformRand()*0.00001;
                double phi = G4UniformRand()*360*degree;
                double sintheta = sqrt(1-costheta*costheta);
                pickinfo.ParticleDirection = G4ThreeVector(sintheta*cos(phi), sintheta*sin(phi), costheta);
        //        pickinfo.ParticleDirection = G4ThreeVector(0, 0, 1);

                generateP0(&pickinfo);

                tbjcEventAction::EventAction->updateBeamType(generateType);
                break;
            }

        case 1:
            {
                cout<<1<<endl;
                pickinfo.ParticleComp = tbjclg->generateBeam();
                pickinfo.K = kin->K3;

                G4ThreeVector ParticleDirection;
                ParticleDirection.setX(sin(kin->thetalab3)*cos(kin->philab3));
                ParticleDirection.setY(sin(kin->thetalab3)*sin(kin->philab3));
                ParticleDirection.setZ(cos(kin->thetalab3));
                pickinfo.ParticleDirection = ParticleDirection;

                pickinfo.ParticlePosition = startpoint;
             //   pickinfo.ParticlePosition = G4ThreeVector(0,0,-10*cm);

                generateP0(&pickinfo);

                tbjcEventAction::EventAction->updateBeamType(generateType);
                generateType--;
                break;
            }


    }


    fParticleGun->GeneratePrimaryVertex(anEvent);

}



int  tbjcPrimaryGeneratorAction::generateP0(const BEAMCOMPINFO * const pickinfo)
{

    cout<<"energy:"<<pickinfo->K
        <<";position:("
        <<pickinfo->ParticlePosition.x()/cm<<","
        <<pickinfo->ParticlePosition.y()/cm<<","
        <<pickinfo->ParticlePosition.z()/cm<<")"

        <<";direction:("
        <<pickinfo->ParticleDirection.x()<<","
        <<pickinfo->ParticleDirection.y()<<","
        <<pickinfo->ParticleDirection.z()<<")"

        <<endl;

    BEAMCOMP* pick = pickinfo->ParticleComp;


    G4ParticleDefinition* particle =  tbjctable->GetIon(pick->Z,pick->A,pick->Ex);


    tbjcEventAction::EventAction->updateenergy(pickinfo->K*MeV);

    fParticleGun->SetParticlePosition(pickinfo->ParticlePosition);

    fParticleGun->SetParticleMomentumDirection(pickinfo->ParticleDirection);


    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(pickinfo->K*MeV);


    return 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

