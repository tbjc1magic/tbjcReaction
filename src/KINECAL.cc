#include "KINECAL.hpp"
#include <iostream>
#include "constants.hpp"
#include "P4VECTOR.hpp"
#include "AMATRIX.hpp"
#include <cmath>
#include <cstdlib>
using namespace std;

KINECAL::KINECAL()
{
    reset();
}

int KINECAL::reset()
{
    for(int i=0;i<4;i++)
    {m[i]=0;}

    Eex2=Eex3=0;
    K2=K3=0;
    V2=V3=0;
    thetalab2=thetalab3=philab2=philab3=0;

    thetaCMS=0;phiCMS=0;

    m[2]=m[0]=m[3]=m[1]=0;
    K0=0;

    return 1;
}

int KINECAL::clear()
{

    K2=K3=0;
    V2=V3=0;

    thetaCMS=0;phiCMS=0;

    thetalab2=thetalab3=philab2=philab3=0;
    return 1;
}


int KINECAL::print()
{
    cout<<"With "<<K0<<" MeV energy Projectile"<<endl;
    cout<<"you got "<< K2<<" MeV  for particle 2; at "<<thetalab2<<":"<<philab2<<endl;
    cout<<"and "<< K3<<" MeV for particle 3; at "<<thetalab3<<":"<<philab3<<endl;
    return 1;

}

int KINECAL:: randomgenerate()
{

    clear();
    float cos_theta=(rand()%200000-100000)/100000.0;
    float phi=(rand()%100000)/100000.0*2*M_PI;
    float theta=acos(cos_theta);
    thetaCMS= theta*180/M_PI; phiCMS=phi*180/M_PI;
    calculate(theta,phi);

    return 1;
}


int KINECAL:: calculate(double theta, double phi)
{
    

    double m0_tmp=m[0]*constant_u;
    double tbjcm1_tmp=m[1]*constant_u;
    double Ek_tmp=K0*constant_MeV;
    double tbjcm2_tmp = m[2]*constant_u;
    double tbjcm3_tmp = m[3]*constant_u;

    double v0_tmp=Ek2v(m0_tmp,Ek_tmp);
//   cout<<"v0"<<v0_tmp<<endl;
    P4VECTOR* P4Va=new P4VECTOR(m0_tmp,v0_tmp);
    
    double u_tmp=findCMSu(m0_tmp,tbjcm1_tmp,v0_tmp);
  

 //   cout<<"relative speed"<<u_tmp<<endl;


    double beta_tmp = u_tmp/constant_c;
    ////A transform X to X'////
    ////_A transform X' to X///
    AMATRIX* A_tmp=new AMATRIX(beta_tmp);
    AMATRIX* _A_tmp=new AMATRIX(-beta_tmp);

//    P4Va->print();
    P4Va->boost(A_tmp);
//    P4Va->print();


    P4VECTOR* P4Vb=new P4VECTOR(tbjcm1_tmp,0);
    
//    P4Vb->print();
    P4Vb->boost(A_tmp);
 //   P4Vb->print();

    double energyA_tmp = (*P4Va)(0)*constant_c;
    double energyB_tmp = (*P4Vb)(0)*constant_c;

    double DeltaE_tmp = (Eex2+Eex3)*constant_MeV;

    double Etot_tmp = energyA_tmp + energyB_tmp - DeltaE_tmp;
   

    //cout<<"Etot "<<Etot_tmp<<endl;


    double E2_0_tmp = tbjcm2_tmp*constant_c*constant_c;
    double E3_0_tmp = tbjcm3_tmp*constant_c*constant_c;


    //cout<<"E2_0 "<<E2_0_tmp<<endl;
    //cout<<"E3_0 "<<E3_0_tmp<<endl;

    double tmp1 = E3_0_tmp*E3_0_tmp + E2_0_tmp*E2_0_tmp - Etot_tmp*Etot_tmp;

    //if(tmp1>0){cout<<"there is no suck thing"<<endl;return 0;}
    

    double tmp2 = 2*E3_0_tmp*E2_0_tmp;


    //cout<<"tmp1 "<<tmp1<<endl;
    //cout<<"tmp2 "<<tmp2<<endl;


    double Pcms2_tmp =(tmp1*tmp1-tmp2*tmp2)/(4*Etot_tmp*Etot_tmp);

    if(Pcms2_tmp<0) {cout<<"OK too low in energy"<<endl;return 0;}
    //cout<<"Pcms2:"<<Pcms2_tmp<<endl;

    double Pcms_tmp = sqrt(Pcms2_tmp)/constant_c;

    //cout<<"Pcms:"<<Pcms_tmp<<endl;

    //cout<<"energy A: "<<energyA_tmp<<endl;
    //cout<<"energy B: "<<energyB_tmp<<endl;

    P4VECTOR* P4Vc=new P4VECTOR();
    P4VECTOR* P4Vd=new P4VECTOR();
   

    double E3_tmp= sqrt( E3_0_tmp*E3_0_tmp+ Pcms_tmp*Pcms_tmp*constant_c*constant_c);
    double E2_tmp= sqrt( E2_0_tmp*E2_0_tmp+ Pcms_tmp*Pcms_tmp*constant_c*constant_c);

    //cout<<"E2r "<<E2_tmp/constant_MeV<<endl;
    //cout<<"E3r "<<E3_tmp/constant_MeV<<endl;



    double PcmsZ_tmp=Pcms_tmp*cos(theta);
    double PcmsX_tmp=Pcms_tmp*sin(theta)*cos(phi);
    double PcmsY_tmp=Pcms_tmp*sin(theta)*sin(phi);


    P4Vc->setP4(E2_tmp/constant_c,PcmsX_tmp,PcmsY_tmp,PcmsZ_tmp);
    P4Vd->setP4(E3_tmp/constant_c,-PcmsX_tmp,-PcmsY_tmp,-PcmsZ_tmp);



  //  cout<<(*P4Vc)(0)/constant_MeV<<endl;
  //  cout<<(*P4Vd)(0)/constant_MeV<<endl;

    P4Vc->boost(_A_tmp);
    P4Vd->boost(_A_tmp);

    
 //   cout<<(*P4Vc)(0)*constant_c/constant_MeV<<endl;
 //   cout<<(*P4Vd)(0)*constant_c/constant_MeV<<endl;


    double K2_tmp = ((*P4Vc)(0)*constant_c-tbjcm2_tmp*constant_c*constant_c);
    double K3_tmp = ((*P4Vd)(0)*constant_c-tbjcm3_tmp*constant_c*constant_c);

   //cout<<K2_tmp<<endl;
   // cout<<K3_tmp<<endl;


    K2 = K2_tmp/constant_MeV;
    K3 = K3_tmp/constant_MeV;


    double P2_tmp=sqrt(pow((*P4Vc)(1),2)+pow((*P4Vc)(2),2)+pow((*P4Vc)(3),2));
    double P3_tmp=sqrt(pow((*P4Vd)(1),2)+pow((*P4Vd)(2),2)+pow((*P4Vd)(3),2));


  //  P4Vc->print();
  //  P4Vd->print();
  //  cout<<P2_tmp<<endl;
    
    thetalab2=acos((*P4Vc)(3)/P2_tmp);
    philab2=phi;

    thetalab3=acos((*P4Vd)(3)/P3_tmp);  
    philab3=(phi+M_PI)-int((phi+M_PI)/M_PI/2)*2*M_PI;

    delete P4Vd;
    delete P4Vc;
    delete P4Va;
    delete P4Vb;

 //   cout<<"V2 ";
    V2 = Ek2v(tbjcm2_tmp,K2_tmp);
 //   cout<<"V3 ";
    V3 = Ek2v(tbjcm3_tmp,K3_tmp);


    print();
    return 1;
}

double KINECAL:: Ek2v(double m,double Ek)//// convert Ek to speed; unit: m: kg; Ek: kg*tbjcm2/s2
{
    float ratio_tmp=Ek/(m*constant_c*constant_c);
 //   cout<<"ratio "<<ratio_tmp<<endl;
    float v=constant_c*sqrt(1-1/((ratio_tmp+1)*(ratio_tmp+1)));
    return v;
}



double KINECAL:: findCMSu(double tbjcm1, double tbjcm2, double v0) /// unit: m: kg; v: m/s
{

    if(v0>=constant_c)
    {
    cout<<"what the heck are u doing?"<<endl;
    return 0;
    }

    double beta_tmp=v0/constant_c;
    double gamma_tmp=1/sqrt(1-beta_tmp*beta_tmp);    
    double tbjcm1r=tbjcm1*gamma_tmp;
    double u=tbjcm1r*v0/(tbjcm1r+tbjcm2);

    return u;
}






