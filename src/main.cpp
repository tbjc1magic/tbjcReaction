#include <iostream>
#include "AMATRIX.hpp"
#include "FOURVECT.hpp"
#include "P4VECTOR.hpp"
#include <cstdlib>
#include "KINECAL.hpp"
#include "TH2.h"
#include "TFile.h"
#include "HELIOS.hpp"
#include <fstream>
using namespace std;


int main()
{

    HELIOS* helios = new HELIOS();

    helios->SetMagneticField(2.);
    helios->SetArrayPosition(-0.5,-0.10);
    helios->SetChamberSize(1);
    helios->SetIonChamberPos(0.68);
    helios->SetArraySize(0.01);

    KINECAL* kin=new KINECAL();
    /* 
       double mass0 = 29.984903; 
       double mass1 = 1.00782503207; 
       double mass2 = 29.984903; 
       double mass3 = 1.00782503207; 
       double K0 = 300;
     */
    double mass0 = 19.9924401754;
    double mass1 = 4.00260325415;
    double mass2 = 22.9897692809;
    double mass3 = 1.00782503207;
    double K0 = 93;




    kin->setMass(mass0,mass1,mass2,mass3);
    kin->setK0(K0);
    kin->calculate(3.1415926/2,1.2);
    kin->print();

    TFile*a= new TFile("output.root","recreate");


    TH2F* h2 = new TH2F("h2","h2",1024,0,5.5,1024,65,95); 
    TH2F* h22 = new TH2F("h22","h22",1024,0,190,1024,0,35); 



    TH2F* EZ = new TH2F("EZ","EZ",1024,-60,-10,1024,2,10);
    TH2F* ET = new TH2F("ET","ET",1024,0,200,1024,2,12);

    TH2F* h222 = new TH2F("h222","h222",1000,0,20,1000,0,100);
    TH2F* h2222 = new TH2F("h2222","h2222",1000,0,20,1000,0,100);


    double ex[6]={0,0.74,0,0,0,0};

    double oldex=-1;

    for(int kk=0;kk<6;kk++)
    {
        if(ex[kk]<=oldex)break;

        kin->setEx2(ex[kk]);
        char buffer[50];
        sprintf(buffer,"ex%d.dat",kk);
        ofstream ff(buffer);

        for(int i=0;i<10000;i++)
        {
            kin->randomgenerate();
            h2->Fill(kin->thetalab2/M_PI*180,kin->K2);
            h22->Fill(kin->thetalab3/M_PI*180,kin->K3);
            int success1 = helios->CalculateZ(kin->thetalab3,kin->V3,1,mass3);

            int success2 = helios->CalculateR(kin->thetalab2,kin->V2,10,mass2);
            if(success1&&success2)
            {
                double arraytimenano = helios->ArrayTime*1e9;
                if(arraytimenano<80)
                {
                    ff<<helios->ArrayZ*100<<" "<<arraytimenano<<" "<<kin->K3<<" "; 
                    ET->Fill(arraytimenano,kin->K3);
                    EZ->Fill(helios->ArrayZ*100,kin->K3);

                    double ionradiuscm = helios->IonR*100;
                    double iontimenano = helios->IonTime*1e9;
                    ff<<ionradiuscm<<" "<<iontimenano<<" "<<kin->K2;
                    h222->Fill(ionradiuscm,kin->K2);
                    h2222->Fill(tan(kin->thetalab2)*120,kin->K2);

                    ff<<endl;

                }
            }
            /*
               if(success2)
               {
               double ionradiuscm = helios->IonR*100;
               double iontimenano = helios->IonTime*1e9;
               ff<<ionradiuscm<<" "<<iontimenano<<" "<<kin->K2;
               h222->Fill(ionradiuscm,kin->K2);
               h2222->Fill(tan(kin->thetalab2)*120,kin->K2);
               }
             */
            //    ff<<endl;

        }
        ff.close();

    }




    a->Write();
    return 1;
}


