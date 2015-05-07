#ifndef __TBJCBEAM__
#define __TBJCBEAM__
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>
#include <string>
#include <random>
using namespace std;
class TBJCBEAM
{

    public:
        double x,y,z;
        TBJCBEAM();
        ~TBJCBEAM();
        int AnotherParticle();
        int setBeamDistributionType(int type);
        int setRadius(double Xr, double Yr, double Zr);
        int setCenter(double Xc, double Yc, double Zc);
        int setLimit(double Xl, double Yl, double Zl);

    private:

        int DistributionType;
        int SquareEvenDist();
        int SquareNormalDist();
        int ZeroResults();

        double EvenDist();
        double NormalDist();
        double Calculate1D(int T);
        int CalculateXYZ(int XT, int YT, int ZT);
        int IsInside(int XLT, int YLT, int ZLT);

        double Xradius, Yradius, Zradius;
        double Xcenter, Ycenter, Zcenter;
        double Xlimit, Ylimit, Zlimit;
            
        random_device* rd; 
        normal_distribution<>* tbjcNormal;
        mt19937* gen;
      //  normal_distribution<> d(5,2);

};
#endif
