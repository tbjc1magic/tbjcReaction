#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <fstream>
using namespace std;

const int SecNum =6;

Float_t tbjcRecoil_save[SecNum];
Float_t tbjcBeam_save[SecNum];
Float_t tbjcProton_save[SecNum];
Float_t x_save, y_save, z_save;
Float_t tbjcP_save;
int blocked_save;
Float_t SIenergy_save;
int zerosaves()
{

    for(int i=0;i<SecNum;i++)
    {
        tbjcBeam_save[i] = 0;
        tbjcProton_save[i] = 0;
        tbjcRecoil_save[i] = 0;
    }

    x_save = 0;
    y_save = 0;
    z_save =0 ;
    tbjcP_save = 0;
    blocked_save = 0;
    SIenergy_save = 0;


}

char* returnFileName(char* inputf)
{

    int i=0;
    int c=0;
    while(inputf[i]!=0)
    {
        if(inputf[i]=='.')break;
        c++;
        i++;
    }

    char* return_str = new char[c+6];

    for(int j=0;j<c;j++)
    {
        return_str[j] = inputf[j];
    }
    return_str[c+5]=0;

    return_str[c]='.';
    return_str[c+1]='r';
    return_str[c+2]='o';
    return_str[c+3]='o';
    return_str[c+4]='t';

    return return_str;
}


int main(int argc, char* argv[] )
{


    if(argc<2)
    {
        cout<<"shit, what are u doing?"<<endl;
        return 1;
    }
    int BeamType;
    Float_t tbjcIonEnergies[6];
    Float_t tbjcEnergy;
    Float_t x, y, z;
    int blocked;
    Float_t SIenergy;
    int ReactionHappened=0;

    ifstream RD(argv[1]);

    if(RD.fail()){cout<<"no such shit available"<<endl;return 1;}

    char* outputf = returnFileName(argv[1]);



    TFile *output = new TFile(outputf,"recreate");
    //    TH2F* hIC_E1E2 = new TH2F("hIC_E1E2","hIC_E1E2",500,0,50,500,0,50);
    //    TH2F* hIC_E2E3 = new TH2F("hIC_E2E3","hIC_E2E3",500,0,50,500,0,50);

    //    TH2F* hEZ = new TH2F("hEZ","hEZ",500,-60,-10,120,0,10);


    TTree *tbjctree = new TTree("tbjctree","tbjc's data tree");
    tbjctree->Branch("tbjcRecoil_save",tbjcRecoil_save,"tbjcRecoil_save[5]/F" );
    tbjctree->Branch("tbjcBeam_save",tbjcBeam_save,"tbjcBeam_save[5]/F" );
    tbjctree->Branch("tbjcProton_save",tbjcProton_save,"tbjcProton_save[5]/F" );
    tbjctree->Branch("x_save",&x_save,"x_save/F" );
    tbjctree->Branch("y_save",&y_save,"y_save/F" );
    tbjctree->Branch("z_save",&z_save,"z_save/F" );
    tbjctree->Branch("tbjcP_save",&tbjcP_save,"tbjcP_save/F" );
    tbjctree->Branch("blocked_save",&blocked_save,"blocked_save/I" );
    tbjctree->Branch("SIenergy_save",&SIenergy_save,"SIenergy_save/F" );
    tbjctree->Branch("SIenergy_save",&SIenergy_save,"SIenergy_save/F" );
    tbjctree->Branch("ReactionHappened",&ReactionHappened,"ReactionHappened/I" );

    while(RD>>BeamType>>tbjcIonEnergies[0]>>tbjcIonEnergies[1]>>tbjcIonEnergies[2]>>tbjcIonEnergies[3]>>tbjcIonEnergies[4]>>tbjcIonEnergies[5]>>x>>y>>z>>tbjcEnergy>>blocked>>SIenergy)
    {


        if(BeamType==0)
        {
            tbjctree->Fill();
            zerosaves();

            for(int i=0;i<SecNum;i++)
            {
                tbjcBeam_save[i] = tbjcIonEnergies[i];
            }

        }

        if(BeamType==1)
        {

            for(int i=0;i<SecNum;i++)
            {
                tbjcProton_save[i] = tbjcIonEnergies[i];
                x_save=x;
                y_save=y;
                z_save=z;
                tbjcP_save=tbjcEnergy;
                SIenergy_save = SIenergy;
            }                   

            ReactionHappened = 1;
        }

        if(BeamType==2)
        {

            for(int i=0;i<SecNum;i++)
            {
                tbjcRecoil_save[i] = tbjcIonEnergies[i];
            }             
            blocked_save = blocked;



        }

        //        hIC_E1E2->Fill(-tbjcIonEnergies[2],-tbjcIonEnergies[1]);
        //        hIC_E2E3->Fill(-tbjcIonEnergies[3],-tbjcIonEnergies[2]);
        //        if(BeamType==1)hEZ->Fill(z/10,tbjcEnergy);

    }

    tbjctree->Write();



    return 1;
}
