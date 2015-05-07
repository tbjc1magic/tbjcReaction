#include <fstream>
#include <math>
using namespace std;
int tt()
{


    TFile *cutf = new TFile("tbjccut0.root");
    TCutG *tc=(TCutG*)cutf->Get("tbjccut0");



    ifstream testf("testenergy.dat");

    TFile* outputf = new TFile("output.root","recreate");
    TH1F* testh1= new TH1F("testh1","testh1",1100,-10,100);
    TH1F* h1_thetaCMS= new TH1F("thetaCMS","thetaCMS",180,0,180);
    TH1F* h1_phiCMS= new TH1F("phiCMS","phiCMS",360,0,360);

    TH2F* Z_thetaCMS = new TH2F("Z_thetaCMS","Z_thetaCMS",1024,-80,0,1800,0,180);
    TH2F* Z_thetaCMSg = new TH2F("Z_thetaCMSg","Z_thetaCMSg",1024,-80,0,1800,0,180);

    TH1F* testre= new TH1F("testre","testre",1100,-10,100);
    TH1F* z= new TH1F("z","z",200000,-1000,1000);
    TH1F* lastR= new TH1F("lastR","lastR",20000,-100,100);
    TH2F* prexy= new TH2F("prexy","prexy",800,-4,4,800,-4,4);
    TH2F* postxy= new TH2F("postxy","postxy",800,-4,4,800,-4,4);
    TH2F* lastxy= new TH2F("lastxy","lastxy",1000,-50,50,1000,-50,50);
    TH2F* hEZ = new TH2F("hEZ","hEZ",1000,60,10,1000,0,10);
    TH2F* hEZg = new TH2F("hEZg","hEZg",1000,60,10,1000,0,10);
    TH2F* h2_xz = new TH2F("h2_xz","h2_xz",1000,-60,-10,600,-3,3);
  //  TH3F* lastxyz= new TH3F("lastxyz","lastxyz",1600,-6,6,1600,-8,8,1600,-20,8);
    double prek,postk,re, prex,prey,prez,postx,posty,postz,lastx,lasty,lastz;
    int type;

    float radius=0;
    int i=0;
    double energy;

    const int IonSecNum = 6;

        int tbjcReactionHappened;
        double tbjcBeamIonEnergies[IonSecNum];
        double tbjcRecoilIonEnergies[IonSecNum];
        double tbjcLightIonEnergies[IonSecNum];
        double SiPos[3];
        double tbjcEnergy;
        double tbjcSIenergy;
        double tbjcBlockedPosition_Beam[3];
        double tbjcBlockedPosition_Recoil[3];
        double tbjcBlockedPosition_Light[3];

        double thetaCMS, phiCMS;


    while(
            testf>>tbjcReactionHappened
            >>tbjcBeamIonEnergies[0]>>tbjcBeamIonEnergies[1]>>tbjcBeamIonEnergies[2]
            >>tbjcBeamIonEnergies[3]>>tbjcBeamIonEnergies[4]>>tbjcBeamIonEnergies[5]
            >>tbjcRecoilIonEnergies[0]>>tbjcRecoilIonEnergies[1]>>tbjcRecoilIonEnergies [2]
            >>tbjcRecoilIonEnergies[3]>>tbjcRecoilIonEnergies[4]>>tbjcRecoilIonEnergies [5]
            >>tbjcLightIonEnergies[0]>>tbjcLightIonEnergies[1]>>tbjcLightIonEnergies[2]
            >>tbjcLightIonEnergies[3]>>tbjcLightIonEnergies[4]>>tbjcLightIonEnergies[5]
            >>SiPos[0]>>SiPos[1]>>SiPos[2]
            >>tbjcEnergy>>tbjcSIenergy
            >>tbjcBlockedPosition_Beam[0]>>tbjcBlockedPosition_Beam[1]>>tbjcBlockedPosition_Beam[2]
            >>tbjcBlockedPosition_Recoil[0]>>tbjcBlockedPosition_Recoil[1]>>tbjcBlockedPosition_Recoil[2]
            >>tbjcBlockedPosition_Light[0]>>tbjcBlockedPosition_Light[1]>>tbjcBlockedPosition_Light[2]
            >>thetaCMS>>phiCMS
            )
    {
        if(tbjcReactionHappened)
        {Z_thetaCMS->Fill(SiPos[2]/10,thetaCMS);
        h1_thetaCMS->Fill(thetaCMS);
        h1_phiCMS->Fill(phiCMS);
        }
        radius = sqrt(tbjcBlockedPosition_Recoil[0]* tbjcBlockedPosition_Recoil[0] + tbjcBlockedPosition_Recoil[1]* tbjcBlockedPosition_Recoil[1]); 
        i++;
        if(i%10000==0)cout<<i/10000<<endl;
    //    cout<<re<<endl;

//        if((lastz<-1.7||lastz>-1.67)&&(radius<4.99||radius>5.01)&&(lastz>-714.97&&lastz<-714.94))//&&re<75)
     //   if(type==0&&lastz>-100.86&&lastz<10)
    //   if(tbjcSIenergy>0.1&&radius<12.5)
        if(tc->IsInside(SiPos[2]/10.0,tbjcSIenergy))
        {
            hEZg->Fill(SiPos[2]/10.0,tbjcSIenergy);
            testre->Fill(re);
            testh1->Fill(postk);
            lastR->Fill(radius);
            prexy->Fill(prex,prey); 
            postxy->Fill(postx,posty);
            lastxy->Fill(tbjcBlockedPosition_Light[0]/10,tbjcBlockedPosition_Light[1]/10);
  //      lastxyz->Fill(lastx,lasty,lastz);
  //      z->Fill(prez);
  //      z->Fill(postz);
          z->Fill(tbjcBlockedPosition_Recoil[2]);  
        }
        
   //    if(radius>12.5)
       hEZ->Fill(SiPos[2]/10.0,tbjcSIenergy);
       h2_xz->Fill(SiPos[2]/10.0,SiPos[0]/10.0);
    }

    outputf->Write();

}
