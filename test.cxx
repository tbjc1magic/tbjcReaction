#include <fstream>
#include <math>
using namespace std;
int test()
{
    ifstream testf("testf.dat");

    TFile* outputf = new TFile("output.root","recreate");
    TH1F* testh1= new TH1F("testh1","testh1",1100,-10,100);
    TH1F* testre= new TH1F("testre","testre",1100,-10,100);
    TH1F* z= new TH1F("z","z",200000,-1000,1000);
    TH1F* lastR= new TH1F("lastR","lastR",20000,-100,100);
    TH2F* prexy= new TH2F("prexy","prexy",800,-4,4,800,-4,4);
    TH2F* postxy= new TH2F("postxy","postxy",800,-4,4,800,-4,4);
    TH2F* lastxy= new TH2F("lastxy","lastxy",1600,-6,6,1600,-8,8);
  //  TH3F* lastxyz= new TH3F("lastxyz","lastxyz",1600,-6,6,1600,-8,8,1600,-20,8);
    double prek,postk,re, prex,prey,prez,postx,posty,postz,lastx,lasty,lastz;
    int type;

    float radius=0;
    int i=0;
    double energy;
    while(testf>>type>>lastx>>lasty>>lastz>>energy)
 //   while(testf>>prek>>postk>>re>>prex>>prey>>prez>>postx>>posty>>postz>>lastx>>lasty>>lastz)
    {
        radius = sqrt(lastx*lastx +lasty*lasty); 
        i++;
        if(i%10000==0)cout<<i/10000<<endl;
    //    cout<<re<<endl;

//        if((lastz<-1.7||lastz>-1.67)&&(radius<4.99||radius>5.01)&&(lastz>-714.97&&lastz<-714.94))//&&re<75)
        if(type==0&&lastz>-100.86&&lastz<10)
        {
        testre->Fill(re);
        testh1->Fill(postk);
        lastR->Fill(radius);
        prexy->Fill(prex,prey); 
        postxy->Fill(postx,posty);
        lastxy->Fill(lastx,lasty);
  //      lastxyz->Fill(lastx,lasty,lastz);
  //      z->Fill(prez);
  //      z->Fill(postz);
          z->Fill(lastz);  
        }
    }

    outputf->Write();

}
