#ifndef __KINECAL__
#define __KINECAL__

class KINECAL
{

   ////notation of particle number 1(0,2)3 0 is the projectile////
    public:
    KINECAL();
    ~KINECAL();
    int reset();
    int print();

    ////main function/////////////
    int calculate(double theta, double phi); ///do calculation with theta2 and phi2 in center mass frame and output the results to K3 K2  
  
    ///generate uniform theta and phi and call calculate
    int randomgenerate();


    ////set all values /////////
    inline int setEx2(double ex){Eex2=ex;return 1;}
    inline int setEx3(double ex){Eex3=ex;return 1;}
    inline int setMass(double m0, double m1, double m2, double m3)
    {m[0]=m0;m[1]=m1;m[2]=m2;m[3]=m3; return 1;}  // in Unit
    inline int setK0(double k0){K0=k0; return 1;} //in units of MeV

    double K2,K3; /// kinetic energy for products unit: MeV
    double thetalab2,philab2;///lab angle for particle 2  
    double thetalab3,philab3;/// lab angle for particle 3
    double V2, V3;
    double thetaCMS, phiCMS;

    private:

    int clear(); //clear output

    double Eex2; /// excitation energy for recoil Unit: MeV
    double Eex3; /// excitation energy for light products Unit:MeV
    double m[4]; ///mass for all particles Unit: atomic unit mass u
    double K0; /// initial kinetic energy for projectile Unit: MeV
   
    double findCMSu(double m1, double m2, double v0); /// unit: m: kg; v: m/s
    double Ek2v(double m,double Ek);//// convert Ek to speed; unit: m: kg; Ek: kg*m2/s2

};

#endif
