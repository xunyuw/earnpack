#ifndef _EZ_MYSQL_H_
#define _EZ_MYSQL_H_

#include <mysql.h>
#include <string>

using namespace std;

class CEzMysqlDB
{
    public:
        CEzMysqlDB(); 
        ~CEzMysqlDB(); 

        int connect(const string& host, 
                        const string& user,
                        const string& passwd,
                        const string& db,
                        const unsigned int port = 0
                        );
        void disconnect();

        int startTrans();

        int rollback();

        int commit();
        
        int createTable(const string& tabName, const string& schema );

        int insertRow(const string& tabName, const string& values);
    private:
        MYSQL* conn_;
    
};

#endif

