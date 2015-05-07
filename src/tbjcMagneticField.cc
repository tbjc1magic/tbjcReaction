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
// $Id: tbjcMagneticField.cc 77656 2013-11-27 08:52:57Z gcosmo $
//
/// \file tbjcMagneticField.cc
/// \brief Implementation of the tbjcMagneticField class

#include "tbjcMagneticField.hh"
#include <fstream>
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcMagneticField::tbjcMagneticField()
: G4MagneticField(), fBy(1.0*tesla)
{


    ifstream myfile ("tbjcMeg.txt");

    G4double radius,zposition,B_Axial,B_Rad;
    
    for (int i=0; i<150; i++) 
    {
        for (int j=0; j<150; j++) 
        {
            
   //    cout<<"heihei"<<endl; 
        myfile>>radius>>zposition>>tbjcBz[i][j]>>tbjcBr[i][j];

        }
    }
    


    // define commands for this class
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

tbjcMagneticField::~tbjcMagneticField()
{ 
}

void tbjcMagneticField::GetFieldValue(const G4double point[4],double *bField) const
{



	G4double x,y,z;
	x=point[0];
	y=point[1];
	z=point[2];//-trackercenter;
	
    double tbjcr,tbjczz;
    tbjcr=(int)(sqrt(x*x+y*y)/cm/0.1);
    tbjczz=(int)(point[2]/cm); ///pay attention, this part will work only if 
        
                        //the size is actual size:whole
    
    const int tbjcr1=(int)tbjcr;
    const int tbjczz1=(int)tbjczz;

    double tbjcangle=0;
    if (y>0&&x>0) {tbjcangle=atan(y/x);}
    if (y>0&&x<0) {tbjcangle=3.1415926-atan(-y/x);}
    if (y<0&&x<0) {tbjcangle=3.1415926+atan(y/x);}
    if (y<0&&x>0) {tbjcangle=2*3.1415926-atan(y/x);}


    if(tbjcr1==0)
    {
    

        
        if(tbjczz1>-150&&tbjczz1<150)
        {
            bField[0]=0;
            bField[1]=0;
            bField[2]=tbjcBz[0][(int)fabs(tbjczz1)]*gauss;
            bField[3]=0;
            bField[4]=0;
            bField[5]=0;
    

         //   cout<<tbjcr1<<" "<<(int)fabs(tbjczz1)<<" "<<tbjcBr[tbjcr1][(int)fabs(tbjczz1)]<<" "<<tbjcBz[tbjcr1][(int)fabs(tbjczz1)]<<endl;

            return;
        }

        
             
    }




    if (tbjcr1>0&&tbjcr1<150)
    {
        if(tbjczz1>=0&&tbjczz1<150)
        {
            bField[0]=tbjcBr[tbjcr1][(int)fabs(tbjczz1)]*cos(tbjcangle)*gauss;
            bField[1]=tbjcBr[tbjcr1][(int)fabs(tbjczz1)]*sin(tbjcangle)*gauss;
            bField[2]=tbjcBz[tbjcr1][(int)fabs(tbjczz1)]*gauss;
            bField[3]=0;
            bField[4]=0;
            bField[5]=0;

            

         //   cout<<tbjcr1<<" "<<(int)fabs(tbjczz1)<<" "<<tbjcBr[tbjcr1][(int)fabs(tbjczz1)]<<" "<<tbjcBz[tbjcr1][(int)fabs(tbjczz1)]<<endl;
            
            return;
        }


        if(tbjczz1>-150&&tbjczz1<0)
        {
            bField[0]=-tbjcBr[tbjcr1][(int)fabs(tbjczz1)]*cos(tbjcangle)*gauss;
            bField[1]=-tbjcBr[tbjcr1][(int)fabs(tbjczz1)]*sin(tbjcangle)*gauss;
            bField[2]=tbjcBz[tbjcr1][(int)fabs(tbjczz1)]*gauss;
            bField[3]=0;
            bField[4]=0;
            bField[5]=0;

         //   cout<<tbjcr1<<" "<<(int)fabs(tbjczz1)<<" "<<tbjcBr[tbjcr1][(int)fabs(tbjczz1)]<<" "<<tbjcBz[tbjcr1][(int)fabs(tbjczz1)]<<endl;


            return;
        }

    }


    if(tbjcr1>=150&&tbjcr1<=900&&fabs(tbjczz1)<100)
    {
        
        
        

        if(tbjczz1>=0&&tbjczz1<150)
        {
            
            bField[0]=tbjcBr[149][(int)fabs(tbjczz1)]*cos(tbjcangle)*gauss;
            bField[1]=tbjcBr[149][(int)fabs(tbjczz1)]*sin(tbjcangle)*gauss;
            bField[2]=tbjcBz[149][(int)fabs(tbjczz1)]*gauss;
            bField[3]=0;
            bField[4]=0;
            bField[5]=0;

            

        //    cout<<tbjcr1<<" "<<(int)fabs(tbjczz1)<<" "<<tbjcBr[tbjcr1][(int)fabs(tbjczz1)]<<" "<<tbjcBz[tbjcr1][(int)fabs(tbjczz1)]<<endl;
            return;
        }


        if(tbjczz1>-150&&tbjczz1<0)
        {
            bField[0]=-tbjcBr[149][(int)fabs(tbjczz1)]*cos(tbjcangle)*gauss;
            bField[1]=-tbjcBr[149][(int)fabs(tbjczz1)]*sin(tbjcangle)*gauss;
            bField[2]=tbjcBz[149][(int)fabs(tbjczz1)]*gauss;
            bField[3]=0;
            bField[4]=0;
            bField[5]=0;


            

          //  cout<<tbjcr1<<" "<<(int)fabs(tbjczz1)<<" "<<tbjcBr[tbjcr1][(int)fabs(tbjczz1)]<<" "<<tbjcBz[tbjcr1][(int)fabs(tbjczz1)]<<endl;
            return;
        }

    }


			bField[0]=0;
			bField[1]=0;
			bField[2]=0;
			bField[3]=0;
			bField[4]=0;
			bField[5]=0;

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
