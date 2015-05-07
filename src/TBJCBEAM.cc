#include "TBJCBEAM.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>
#include <string>
#include <random>
using namespace std;
TBJCBEAM::TBJCBEAM()
{

    ////////////////////////////
    ///0 square even dist///////
    ///1 square normal dist/////
    ///2 ball even dist/////////
    ///3 ball normal dist///////
    ////////////////////////////

    DistributionType=0;

    /////////////////////////////

    Xradius = 1; //unit:cm
    Yradius = 1; //unit:cm
    Zradius = 1; //unit:cm

    Xcenter = 0; //unit:cm
    Ycenter = 0; //unit:cm
    Zcenter = 0; //unit:cm

    Xlimit = 1; //unit:cm
    Ylimit = 1; //unit:cm
    Zlimit = 1; //unit:cm



    ZeroResults();

    //////////////////////////////
    srand(time(NULL));
    tbjcNormal = new normal_distribution<>(0,1);
    rd = new random_device;
    gen = new mt19937((*rd)());
}

TBJCBEAM::~TBJCBEAM()
{
    delete tbjcNormal;
    delete gen;
    delete rd;

}

int TBJCBEAM:: setRadius(double Xr, double Yr, double Zr)
{
    Xradius = Xr;
    Yradius = Yr;
    Zradius = Zr;

    return 1;
}

int TBJCBEAM :: setCenter(double Xc, double Yc, double Zc)
{
    Xcenter = Xc;
    Ycenter = Yc;
    Zcenter = Zc;

    return 1;
}



double TBJCBEAM::NormalDist()
{
    return (*tbjcNormal)(*gen);
}


double TBJCBEAM::EvenDist()
{
    double position = rand()%10001/10000.0*2-1;
    return position;
}

int TBJCBEAM:: ZeroResults()
{
    x=y=z=0;
    return 1;
}

double TBJCBEAM:: Calculate1D(int T)
{
    return T?EvenDist():NormalDist();
}

int TBJCBEAM:: CalculateXYZ(int XT, int YT, int ZT)
{
    
    x = Calculate1D(XT) * Xradius;
    y = Calculate1D(YT) * Yradius;
    z = Calculate1D(ZT) * Zradius;

    return 1;
}

int TBJCBEAM:: IsInside(int XLT, int YLT, int ZLT)
{
    if(XLT==0&&fabs(x)>Xlimit) return 0;
    if(YLT==0&&fabs(y)>Ylimit) return 0;
    if(ZLT==0&&fabs(z)>Zlimit) return 0;

    double tmpjudge = (x*x)*(XLT==1)/(Xlimit*Xlimit)+(y*y)*(YLT==1)/(Ylimit*Ylimit)+(z*z)*(ZLT==1)/(Zlimit*Zlimit);
    if(tmpjudge>1)return 0;
   
    return 1;
}

int TBJCBEAM:: SquareEvenDist()
{
    //    cout<<"here is SED"<<endl;
    do
    {
        CalculateXYZ(1,1,1);
    }while(!IsInside(0,0,0));
    
    return 1;
}

int TBJCBEAM:: SquareNormalDist()
{
    //    cout<<"here is SND"<<endl;
    do
    {
        CalculateXYZ(0,0,0);
    }while(!IsInside(0,0,0));
    
    return 1;
}

int TBJCBEAM::AnotherParticle()
{
    ZeroResults();

    switch(DistributionType)
    {
        case 0:
            SquareEvenDist();
            break;
        case 1:
            SquareNormalDist();
            break;
        default:
            break;

    }

    x+=Xcenter;
    y+=Ycenter;
    z+=Zcenter;

    return 1;
}

int TBJCBEAM::setBeamDistributionType(int type)
{
    DistributionType = type;
    return 1;
}


