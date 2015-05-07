#ifndef __ATOMICMASSTABLE__
#define __ATOMICMASSTABLE__
#include <sqlite3.h>
#include <string>
using namespace std;
//const char* AMTname = "AtomicMassTable";
//const char* DBname = "test.db";

class ATOMICMASSTABLE
{

    public:
        ATOMICMASSTABLE(string dbname, string AMTname);
        ~ATOMICMASSTABLE();
 
        double SelectEntry(int A, int Z);
        string ReturnName(int A, int Z); 
        int CheckTableExists(string tablename, sqlite3* db);
        int ReCreateTable(string tablename, string dbname, string textfilename);       
        int OpenDataBase(string DataBaseName);
        int CloseDataBase(string DataBaseName);
    private:

        string TableName;
        sqlite3 *DataBase; 


     //   int CheckTableExistCallback(void *returnvalue, int argc, char **argv, char **azColName);
};

#endif

