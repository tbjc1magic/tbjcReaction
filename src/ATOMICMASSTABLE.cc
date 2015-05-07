#include "ATOMICMASSTABLE.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>     /* atoi */
using namespace std;

ATOMICMASSTABLE::ATOMICMASSTABLE(string dbname, string AMTname)
{
    DataBase = 0;
    TableName="";

    sqlite3 *db;

    char *err_msg = 0;

    int rc = sqlite3_open(dbname.c_str(), &db);


    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(err_msg);
        return;
    }


    rc = CheckTableExists(AMTname, db);


    if(!rc)
    {
        sqlite3_close(db);
    }

    sqlite3_free(err_msg);

    TableName = AMTname;
    DataBase = db;
    cout<<TableName<<endl;
}

ATOMICMASSTABLE::~ATOMICMASSTABLE()
{ 
    if(DataBase)sqlite3_close(DataBase);
}






int ATOMICMASSTABLE::CheckTableExists(string tablename, sqlite3* db)
{


    char CheckTable[] = "SELECT * FROM sqlite_master WHERE type='table' AND name=";
    char *buffer = new char[sizeof(CheckTable)/sizeof(char)+tablename.length()+4];

    sprintf(buffer,"%s'%s';",CheckTable,tablename.c_str());

  //  cout<<buffer<<endl;

    char *err_msg=0;
    int CountsOfTable = 0;


    sqlite3_stmt *stmt = 0;

        sqlite3_prepare(
                db, 
                buffer,  // stmt
                -1, // If than zero, then stmt is read up to the first nul terminator
                &stmt,
                0  // Pointer to unused portion of stmt
                );

    delete buffer;

    while(1)
    {

        int step = sqlite3_step(stmt);

        if (step == SQLITE_ROW) {

            int colcount = sqlite3_column_count(stmt);
        
            sqlite3_finalize(stmt);
            return 1;

            for(int i=0; i<colcount;i++)
            {
                printf("%s: ", sqlite3_column_text(stmt, i));
            }  

        }else if (step == SQLITE_DONE) 
        {
            break;
        }
        else 
        {
            fprintf (stderr, "Failed.\n");
            return 0;
        }

    }


    return 0;

}


int ATOMICMASSTABLE::ReCreateTable(string tablename, string dbname, string textfilename)
{



    if(DataBase)sqlite3_close(DataBase);

    DataBase = 0;
    TableName = "";


    sqlite3_stmt *stmt;
    char *err_msg=0;
    sqlite3 *db;
    char* buffer;
    ////////////////Open a new db////////////////////////////////////
    int rc = sqlite3_open(dbname.c_str(), &db);

    if (rc != SQLITE_OK) 
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(err_msg);
        return 0;
    }



    ////////////////Clear table AtomicMassTable//////////////////////

    char CleanTable[] = "DROP TABLE if EXISTS ";
    buffer = new char[sizeof(CleanTable)/sizeof(char)+tablename.length()+2];

    sprintf (buffer, "%s%s;", CleanTable, tablename.c_str());

    rc = sqlite3_exec(db, buffer, 0, 0, &err_msg);

    delete buffer;

    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);        
        sqlite3_close(db);

        return 0;
    } 


    sqlite3_free(err_msg);

    ////////////////////Create table AtomicMassTable//////////////////////


    char CreateTable1[] = "CREATE TABLE ";
    char CreateTable2[] = "(Id INT PRIMARY KEY, A INT, Z INT, mass DOUBLE,Name TEXT);";
    buffer = new char[sizeof(CreateTable1)/sizeof(char)+tablename.length()+sizeof(CreateTable2)/sizeof(char)+1];

    sprintf (buffer, "%s%s%s", CreateTable1, tablename.c_str(), CreateTable2);


    rc = sqlite3_exec(db, buffer, 0, 0, &err_msg);

    delete buffer;

    if (rc != SQLITE_OK ) 
    {

        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);        
        sqlite3_close(db);

        return 0;
    } 

    sqlite3_free(err_msg);


    rc = CheckTableExists("AtomicMassTable", db);
    if(!rc)cout<<"did not find the table"<<endl;

    /////////////////////Insert table AtomicMassTable//////////////////////


    char InsertElements1[] = "INSERT INTO ";
    char InsertElements2[] = " VALUES(?,?,?,?,?);";

    buffer = new char[sizeof(InsertElements1)/sizeof(char)+tablename.length()+sizeof(InsertElements2)/sizeof(char)+1];
    sprintf (buffer, "%s%s%s", InsertElements1, tablename.c_str(), InsertElements2);

    ifstream inputf(textfilename.c_str());

    int n,z, id=0,a;
    float m;
    string name;


    if(inputf==0)cout<<"shit what is happening"<<endl;

    //  cout<<"hello there"<<endl;


    while(inputf>>n>>z>>m>>name)
    {
        a=n+z;
        cout<<a<<" "<<z<<" "<<m<<" "<<name<<endl;
        id++;


        sqlite3_prepare(
                db, 
                buffer,  // stmt
                -1, // If than zero, then stmt is read up to the first nul terminator
                &stmt,
                0  // Pointer to unused portion of stmt
                );

        sqlite3_bind_int(stmt, 1, id);

        sqlite3_bind_int(stmt,2, a);

        sqlite3_bind_int(stmt, 3, z);

        sqlite3_bind_double(stmt, 4, m);

        sqlite3_bind_text(stmt, 5, name.c_str(), name.length(), 0);


        if (sqlite3_step(stmt) != SQLITE_DONE) 
        {
            fprintf(stderr,"\nCould not step (execute) stmt.\n");
            return 0;
        }

        sqlite3_finalize(stmt);


    }


    TableName = tablename;
    DataBase = db;
}

string ATOMICMASSTABLE::ReturnName(int A, int Z) 
{

    if(!DataBase)
    {
        fprintf(stderr, "SQL error: %s\n", "DataBase not available");
        return "";
    }

    /////////////////////start selecting entry//////////////////

    char SelectTable1[] = "SELECT * FROM ";
    char SelectTable2[] = " WHERE Z=? AND A=?;";
    char *buffer = new char[sizeof(SelectTable1)/sizeof(char) + TableName.length()+ sizeof(SelectTable2)/sizeof(char)+1];
    sprintf(buffer,"%s%s%s",SelectTable1, TableName.c_str(), SelectTable2);


    string returnname = "";

    char *err_msg = 0;


    sqlite3_stmt* stmt;


    sqlite3_prepare(
            DataBase, 
            buffer,  // stmt
            -1, // If than zero, then stmt is read up to the first nul terminator
            &stmt,
            0  // Pointer to unused portion of stmt
            );

    sqlite3_bind_int(stmt, 1, Z);

       sqlite3_bind_int(stmt,2, A);


    while(1)
    {

        int step = sqlite3_step(stmt);

        if (step == SQLITE_ROW) {

            int colcount = sqlite3_column_count(stmt);
          
            returnname = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            /*
            for(int i=0; i<colcount;i++)
            {
                printf("%s: ", sqlite3_column_text(stmt, i));
            }
            */

        }else if (step == SQLITE_DONE) 
        {
            break;
        }
        else 
        {
            fprintf (stderr, "Failed.\n");
            return "";
        }

    }

    sqlite3_finalize(stmt);

    return returnname;


}



double ATOMICMASSTABLE::SelectEntry(int A, int Z)
{


    if(!DataBase)
    {
        fprintf(stderr, "SQL error: %s\n", "DataBase not available");
        return 0;
    }

    /////////////////////start selecting entry//////////////////

    char SelectTable1[] = "SELECT * FROM ";
    char SelectTable2[] = " WHERE Z=? AND A=?;";
    char *buffer = new char[sizeof(SelectTable1)/sizeof(char) + TableName.length()+ sizeof(SelectTable2)/sizeof(char)+1];
    sprintf(buffer,"%s%s%s",SelectTable1, TableName.c_str(), SelectTable2);


    double returnvalue=-1;

    char *err_msg = 0;


    sqlite3_stmt* stmt;


    sqlite3_prepare(
            DataBase, 
            buffer,  // stmt
            -1, // If than zero, then stmt is read up to the first nul terminator
            &stmt,
            0  // Pointer to unused portion of stmt
            );

    sqlite3_bind_int(stmt, 1, Z);

       sqlite3_bind_int(stmt,2, A);


    while(1)
    {

        int step = sqlite3_step(stmt);

        if (step == SQLITE_ROW) {

            int colcount = sqlite3_column_count(stmt);
          
            returnvalue = sqlite3_column_double(stmt, 3);
            /*
            for(int i=0; i<colcount;i++)
            {
                printf("%s: ", sqlite3_column_text(stmt, i));
            }
            */

        }else if (step == SQLITE_DONE) 
        {
            break;
        }
        else 
        {
            fprintf (stderr, "Failed.\n");
            return -1;
        }

    }

    sqlite3_finalize(stmt);

    return returnvalue;

}

