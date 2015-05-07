#include "TBJCINTERFACE.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <utility>      // std::pair

#include "G4SystemOfUnits.hh"
using namespace std;

TBJCINTERFACE* TBJCINTERFACE::interface = 0;

TBJCINTERFACE:: TBJCINTERFACE()
{
    interface = this;

}

TBJCINTERFACE:: ~TBJCINTERFACE()
{


}


int TBJCINTERFACE::ReadParameterFile(string fname)
{

    ifstream ff(fname);
    std::string str((std::istreambuf_iterator<char>(ff)),
            std::istreambuf_iterator<char>());


    json_object* allstr = json_tokener_parse(str.c_str());


    enum json_type type;


    json_object_object_foreach(allstr,key,val)
    {
    //    cout<<key<<endl;
        if(strcmp(key,"Reaction")==0)ReadReactionAttributes(val);
        if(strcmp(key,"Target")==0)ReadTargetAttributes(val);
        if(strcmp(key,"IonChamber")==0)ReadIonChamberAttributes(val);
        if(strcmp(key,"HELIOS")==0)ReadHELIOSAttributes(val);
    }

    return 1;
}


int TBJCINTERFACE::addEx2(double Ex2, double Chance)
{
    ExChance[2].push_back(make_pair(Ex2, Chance));


    return 2;

}
int TBJCINTERFACE::PrintOut()
{

    cout<<"*******Reaction*****"<<endl;
    cout<<"beam energy:"<<BeamK0<<endl;
    for(int i=0;i<4;i++)
    {
        cout<<"particle :A="<<tbjcA[i]<<" Z="<<tbjcZ[i]<<endl;
        for(vector<pair<double, double> >::iterator it = ExChance[i].begin();it != ExChance[i].end(); it++)
        {
            cout<<it->first<<":"<<it->second<<endl;
        }
    }


    cout<<"*******Target*******"<<endl;

    cout<<TargetType<<":"<<tbjcTargetMaterial<<endl;

    for(auto it = TargetAttributes.begin(); it!= TargetAttributes.end(); it++)
    {
        cout<<it->first<<":"<<it->second<<endl;
    }


    cout<<"*******IonChamber*******"<<endl;

    cout<<tbjcTargetMaterial<<endl;

    for(auto it = IonChamberAttributes.begin(); it!= IonChamberAttributes.end(); it++)
    {
        cout<<it->first<<":"<<it->second<<endl;
    }


    cout<<"*******HELIOS*******"<<endl;


    for(auto it = HELIOSAttributes.begin(); it!= HELIOSAttributes.end(); it++)
    {
        cout<<it->first<<":"<<it->second<<endl;
    }


    return 1;
}

/*
   const double mm=0.001;
   const double um=1e-6;
   const double m=0.001;
   const double cm=0.01;
   const double torr = 133.3;
   const double K=1;
   const double inch=2.54*cm;
   const double MeV=1;
 */

namespace TBJCINTERFACESPACE
{
    //const double mm=0.001;
    //const double um=1e-6;
    //const double m=0.001;
    //const double cm=0.01;
    const double torr = 133.3;
    const double K=1;
    const double inch=2.54*cm;
    const double T = 1;
    //const double MeV=1;



}


double TBJCINTERFACE:: String2Number(string input)
{
    int starpos = input.find('*');

    if(starpos<0) return -1e18;

    string PureNumber = input.substr(0,starpos);
    string unitstr = input.substr(starpos+1,input.length()-starpos-1);

    double unit = -1;

    if(strcmp(unitstr.c_str(),"inch")==0) unit = TBJCINTERFACESPACE::inch;
    if(strcmp(unitstr.c_str(),"mm")==0) unit =mm;
    if(strcmp(unitstr.c_str(),"um")==0) unit =um;
    if(strcmp(unitstr.c_str(),"cm")==0) unit =cm;
    if(strcmp(unitstr.c_str(),"torr")==0) unit =TBJCINTERFACESPACE::torr;
    if(strcmp(unitstr.c_str(),"K")==0) unit =TBJCINTERFACESPACE::K;
    if(strcmp(unitstr.c_str(),"m")==0) unit =m;
    if(strcmp(unitstr.c_str(),"MeV")==0) unit =1;
    if(strcmp(unitstr.c_str(),"T")==0) unit =TBJCINTERFACESPACE::T;


    if(unit<0)
    {

        fprintf (stderr, "*************************************\n");
        fprintf (stderr, "Error, unit is way wrong.\n");
        fprintf (stderr, "*************************************\n");

    }

    //  cout<<"cm "<<cm<<endl;
    //  cout<<"mm "<<mm<<endl;

    //    cout<<PureNumber<<"*"<<unit<<endl;

    return stod(PureNumber)*unit;


}




int TBJCINTERFACE:: ReadReactionAttributes(json_object* inputobj)
{

    json_object* ElementsObj = json_object_object_get(inputobj,"Elements");



    if(json_object_get_type(ElementsObj)!=json_type_null)
    {

        json_object_object_foreach(ElementsObj,key,val)
        {

            int index = key[1]-'0';

            json_object* Aobj = json_object_object_get(val,"A");
            int A = json_object_get_int(Aobj);
            json_object* Zobj = json_object_object_get(val,"Z");
            int Z = json_object_get_int(Zobj);

            tbjcA[index]=A;
            tbjcZ[index]=Z;

            json_object* ExChanceArr = json_object_object_get(val,"ExChance");

            if(json_object_get_type(ExChanceArr)== json_type_array )
            {
                int length = json_object_array_length(ExChanceArr);

                for(int i=0; i<length; i++)
                {
                    json_object* ExChancePair = json_object_array_get_idx(ExChanceArr, i);

                    json_object* Exobj = json_object_object_get(ExChancePair,"Ex");
                    double Ex = json_object_get_double(Exobj);
                    json_object* Chanceobj = json_object_object_get(ExChancePair,"Chance");
                    double Chance = json_object_get_double(Chanceobj);

                    ExChance[index].push_back(make_pair(Ex,Chance));
                }

            }
        }
    }

    json_object* BeamEnergyObj = json_object_object_get(inputobj,"BeamEnergy");

    if(json_object_get_type(BeamEnergyObj)!=json_type_null)
    {
        BeamK0 = String2Number(json_object_get_string(BeamEnergyObj));

    } 



    return 1;
}



int TBJCINTERFACE:: ReadHELIOSAttributes(json_object* inputobj)
{
    json_object_object_foreach(inputobj,key,val)
    {

        if(json_object_get_type(val)==json_type_string)
        {
                double value = String2Number(json_object_get_string(val));

                if(value>-1e17)
                {
                    HELIOSAttributes.insert(make_pair(key,value)); 
                }
        }
        else
        {
                double value = json_object_get_double(val);
                HELIOSAttributes.insert(make_pair(key,value)); 
        }

    } 


    return 1;
}


int TBJCINTERFACE:: ReadTargetAttributes(json_object* inputobj)
{
    int NumberOfTargets = json_object_object_length(inputobj);

    if(NumberOfTargets!=1)
    {
        fprintf (stderr, "*************************************\n");
        fprintf (stderr, "Error, Only one target can be accept.\n");
        fprintf (stderr, "*************************************\n");
        exit(1);
    }


    json_object_object_foreach(inputobj,keyprime,valprime)
    {

        TargetType = string(keyprime);

        json_object_object_foreach(valprime,key,val)
        {


            if(json_object_get_type(val)==json_type_string)
            {
                double value = String2Number(json_object_get_string(val));

                if(value<-1e17)
                {
                    tbjcTargetMaterial = json_object_get_string(val);
                }

                if(value>-1e17)
                {
                    TargetAttributes.insert(make_pair(key,value)); 
                }
            }
            else
            {
                double value = json_object_get_double(val);
                TargetAttributes.insert(make_pair(key,value)); 
            }



        } 
    }


    return 1;

}


int TBJCINTERFACE:: ReadIonChamberAttributes(json_object* inputobj)
{

    json_object_object_foreach(inputobj,key,val)
    {

        double value = String2Number(json_object_get_string(val));

        if(value<0)
        {
            tbjcIonChamberMaterial = json_object_get_string(val);
        }

        if(value>0)
        {
            IonChamberAttributes.insert(make_pair(key,value)); 
        }



    } 


    return 1;

}


int TBJCINTERFACE::GetAttributes(string valuename, void* ptr)
{

    ///////Reaction///////////
    if(strcmp(valuename.c_str(),"A0")==0){*(int*)ptr=tbjcA[0];return 1;}
    if(strcmp(valuename.c_str(),"Z0")==0){*(int*)ptr=tbjcZ[0];return 1;}
    if(strcmp(valuename.c_str(),"A1")==0){*(int*)ptr=tbjcA[1];return 1;}
    if(strcmp(valuename.c_str(),"Z1")==0){*(int*)ptr=tbjcZ[1];return 1;}
    if(strcmp(valuename.c_str(),"A2")==0){*(int*)ptr=tbjcA[2];return 1;}
    if(strcmp(valuename.c_str(),"Z2")==0){*(int*)ptr=tbjcZ[2];return 1;}
    if(strcmp(valuename.c_str(),"A3")==0){*(int*)ptr=tbjcA[3];return 1;}
    if(strcmp(valuename.c_str(),"Z3")==0){*(int*)ptr=tbjcZ[3];return 1;}

    if(strcmp(valuename.c_str(),"BeamEnergy")==0){*(double*)ptr=BeamK0;return 1;} 
    if(strcmp(valuename.c_str(),"Ex2Chance")==0){*(vector<pair<double,double> >*)ptr=ExChance[2];return 1;}


    ///////Target/////////////
    if(strcmp(valuename.c_str(),"TargetType")==0){*(string*)ptr=TargetType;return 1;}
    if(strcmp(valuename.c_str(),"TargetMaterial")==0){*(string*)ptr=tbjcTargetMaterial;return 1;}
  
    auto it = TargetAttributes.find(valuename);
    if(it!=TargetAttributes.end())
    {

        *(double*)ptr = it->second;
        return 1;
    }

    ///////IonChamber//////////

    if(strcmp(valuename.c_str(),"IonChamberGasType")==0){*(string*)ptr=tbjcIonChamberMaterial;return 1;}

    auto iit = IonChamberAttributes.find(valuename);
    if(iit!=IonChamberAttributes.end())
    {
        *(double*)ptr = iit->second;
        return 1;
    }

    ///////HELIOS//////////


    auto iiit = HELIOSAttributes.find(valuename);
    if(iiit!=HELIOSAttributes.end())
    {
        *(double*)ptr = iiit->second;
        return 1;
    }



    return 0;
}


