
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TH2.h"
using namespace std;

const int SecNum =5;

Float_t tbjcRecoil_save[SecNum];
Float_t tbjcBeam_save[SecNum];
Float_t tbjcProton_save[SecNum];
Float_t x_save, y_save, z_save;
Float_t tbjcP_save;
int blocked_save;
Float_t SIenergy_save;
int ReactionHappened;

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
    z_save =0;
    tbjcP_save = 0;
    blocked_save=0;
    SIenergy_save = 0;
    ReactionHappened = 0;


    return 1;

}

int print()
{

    for(int i=0;i<SecNum;i++)
    {
        cout<<tbjcRecoil_save[i]<<" ";
    }

    for(int i=0;i<SecNum;i++)
    {
        cout<<tbjcBeam_save[i]<<" ";
    }

    for(int i=0;i<SecNum;i++)
    {
        cout<<tbjcProton_save[i]<<" ";
    }

    cout<<x_save<<" ";
    cout<<y_save<<" ";
    cout<<z_save<<" ";
    cout<<tbjcP_save<<" ";

    cout<<endl;

    return 1;
}


struct Point
{
    double x;
    double y;
};

struct Rect
{
    Point p1; //lower left
    Point p2; //upper right
};


int IsInsideRect(Point pp, Rect rr)
{


    if(pp.x>=rr.p1.x&&pp.y>=rr.p1.y&&pp.x<=rr.p2.x&&pp.y<=rr.p2.y)
    {
        return 1;
    }
    return 0;
}


int IsinsideArray(Point pp, Rect *rr)
{

    for(int i=0;i<6;i++)
    {

        //      cout<<rr[i].p1.x<<" "<<rr[i].p1.y<<" ";
        //      cout<<rr[i].p2.x<<" "<<rr[i].p2.y<<" ";
        //      cout<<endl;
    }


    int NumOfArray =6;

    for(int i=0;i<NumOfArray;i++)
    {

        if(IsInsideRect(pp,rr[i]))return 1;

    }

    return 0;
}

TH2F* hIC_E1E2;// = new TH2F("hIC_E1E2","hIC_E1E2",500,0,50,500,0,50);
TH2F* hIC_E2E3;// = new TH2F("hIC_E2E3","hIC_E2E3",500,0,50,500,0,50);
TH2F* hEZ;// = new TH2F("hEZ","hEZ",500,-60,-10,120,0,10);


TFile *cut2 = new TFile("tbjccut2.root");
TCutG * tbjccut2 = (TCutG*)cut2->Get("tbjccut2");

ofstream outline1("line2.dat");

int ttfill()
{



    Point ptest;
    Rect rtest;

    rtest.p1.x=0;rtest.p1.y=0;rtest.p2.x=1;rtest.p2.y=1;
    ptest.x=0.5;ptest.y=0.5;

    cout<<IsInsideRect(ptest,rtest)<<endl;


    Point sp1,sp2;
    sp1.x = 20+0.3;sp1.y = -0.45;
    sp2.x = 20+0.3+5.05;sp2.y = 0.45;

    Rect tbjcRect[6];

    for(int i=0;i<6;i++)
    {
        tbjcRect[i].p1=sp1;
        tbjcRect[i].p2=sp2;

        sp1.x += 0.3+5.05;
        sp2.x += 0.3+5.05;
    }

    for(int i=0;i<6;i++)
    {

        cout<<tbjcRect[i].p1.x<<" "<<tbjcRect[i].p1.y<<" ";
        cout<<tbjcRect[i].p2.x<<" "<<tbjcRect[i].p2.y<<" ";
        cout<<endl;
    }









    tbjctree->SetBranchAddress("tbjcBeam_save",tbjcBeam_save);
    tbjctree->SetBranchAddress("tbjcRecoil_save",tbjcRecoil_save);
    tbjctree->SetBranchAddress("tbjcProton_save",tbjcProton_save);
    tbjctree->SetBranchAddress("tbjcP_save",&tbjcP_save);
    tbjctree->SetBranchAddress("x_save",&x_save);
    tbjctree->SetBranchAddress("y_save",&y_save);
    tbjctree->SetBranchAddress("z_save",&z_save);
    tbjctree->SetBranchAddress("blocked_save",&blocked_save);
    tbjctree->SetBranchAddress("SIenergy_save",&SIenergy_save);
    tbjctree->SetBranchAddress("ReactionHappened",&ReactionHappened);




    int NumOfEntries = tbjctree->GetEntries();

    int reactioncounts = 0;

    for(int i=0;i<NumOfEntries;i++)
    {
        zerosaves();
        tbjctree->GetEntry(i);


        if(ReactionHappened)
        {
            reactioncounts++;
            cout<<reactioncounts<<endl;
        }

        int inside =0;

        if(fabs(x_save)>10)
        {
            Point ip;ip.y=-y_save/10;ip.x=-z_save/10;
            inside = IsinsideArray(ip,tbjcRect);
            //   cout<<ip.x<<" "<<ip.y<<endl;




        }
        else
        {
            if(fabs(y_save)>10)
            {
                Point ip;ip.y=-x_save/10;ip.x=-z_save/10;
                inside = IsinsideArray(ip,tbjcRect);
                //      cout<<ip.x<<" "<<ip.y<<endl;
                
            }
 

        }

  //   if(blocked_save)
      //  if(blocked_save)
  //      if(SIenergy_save>0.1)
//        if(tbjccut2->IsInside(z_save/10,SIenergy_save))
        {
          outline1<<z_save/10<<" "<<SIenergy_save<<endl;
     //       cout<<x_save<<" "<<y_save<<" "<<z_save<<endl;
            hIC_E1E2->Fill(-tbjcRecoil_save[2],-tbjcRecoil_save[0]);
            hIC_E1E2->Fill(-tbjcBeam_save[2],-tbjcBeam_save[0]);
            hIC_E2E3->Fill(-tbjcRecoil_save[3],-tbjcRecoil_save[2]);
            hIC_E2E3->Fill(-tbjcBeam_save[3],-tbjcBeam_save[2]);
            hEZ->Fill(z_save/10,SIenergy_save);
        }


    }


}


TTree* tbjctree;// = (TTree*)ff->Get("tbjctree");

int reader()
{

    //  ptest.x=30.8;ptest.y=0.1;
    //   cout<<"array "<<IsinsideArray(ptest,tbjcRect)<<endl;





    TFile* of = new TFile("tbjc.root","recreate");
    hIC_E1E2 = new TH2F("hIC_E1E2","hIC_E1E2",500,0,50,500,0,50);
    hIC_E2E3 = new TH2F("hIC_E2E3","hIC_E2E3",500,0,50,500,0,50);
    hEZ = new TH2F("hEZ","hEZ",5000,-60,-10,1000,0,10);


    TFile* ff;
/*
    TFile* ff = new TFile("ap_0_100MeV_2%5_64cm.root");
    tbjctree = (TTree*)ff->Get("tbjctree");
    ttfill(); 


    TFile* ff = new TFile("ap_2%9_100MeV_2%5_64cm.root");
    tbjctree = (TTree*)ff->Get("tbjctree");
    ttfill(); 
*/

    TFile* ff = new TFile("out3.root");
    tbjctree = (TTree*)ff->Get("tbjctree");
    ttfill(); 

   /* 
    
    TFile* ff = new TFile("dp_0%3_100MeV_2%5_64cm.root");
    tbjctree = (TTree*)ff->Get("tbjctree");
    ttfill(); 

    TFile* ff = new TFile("dp_2%8_100MeV_2%5_64cm.root");
    tbjctree = (TTree*)ff->Get("tbjctree");
    ttfill();
*/

   /* 
    TFile* ff = new TFile("reaction_2_100MeV_2%5_64cm.root");
    tbjctree = (TTree*)ff->Get("tbjctree");
    ttfill(); 


    TFile* ff = new TFile("reaction_3_100MeV_2%5_64cm.root");
    tbjctree = (TTree*)ff->Get("tbjctree");
    ttfill(); 
    */










    of->Save();
    of->Write();

    return 1;

}



