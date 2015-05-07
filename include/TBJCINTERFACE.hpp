#ifndef __TBJCINTERFACE__
#define __TBJCINTERFACE__
#include <vector>
#include <json/json.h>
#include <string>
#include <map>
using namespace std;
class TBJCINTERFACE
{

    public:

        static:: TBJCINTERFACE* interface;

        TBJCINTERFACE();
        ~TBJCINTERFACE();
        int addEx2(double, double);
        int PrintOut();


        int ReadParameterFile(string fname);

        int GetAttributes(string valuename, void* ptr);

    private:

        ////////Reaction Elements///////
     
        int tbjcA[4], tbjcZ[4];
        double BeamK0;
        vector<pair<double, double> > ExChance[4];

        /////////Target//////////////////

        string TargetType;
        string tbjcTargetMaterial;
        map<string, double> TargetAttributes;

        /////////Ion Chamber//////////////////

        string tbjcIonChamberMaterial;
        map<string, double> IonChamberAttributes;

        /////////Ion Chamber//////////////////

        map<string, double> HELIOSAttributes;

        ////////Private Functions//////////
        int ReadIonChamberAttributes(json_object* inputobj);
        int ReadHELIOSAttributes(json_object* inputobj);
        int ReadTargetAttributes(json_object* inputobj);
        int ReadReactionAttributes(json_object* inputobj);
        double String2Number(string input);
};

#endif

