#ifndef _EZ_MYSQL_H_
#define _EZ_MYSQL_H_

#include <mysql.h>
#include <string>
#include <vector>

using namespace std;

//class CEzMysqlDB;

class CEzMysqlQ
{
    //friend class CEzMysqlDB;
    public:
        explicit CEzMysqlQ(MYSQL* conn) :_conn(conn), _res(NULL) { };

        ~CEzMysqlQ() 
        {
            close();
        };
        
        void close()
        {
            if (NULL != _res )
            {
                mysql_free_result(_res);
                _res = NULL;
            }
        };

       /* bool execute(const string& sql); 
        bool execute(const char* const sql); */

        bool execute( const char* fmt, ...);
        unsigned long get_ele_len(int idx);
        MYSQL_ROW fetchone();
        vector<MYSQL_ROW> fetchall();

        int get_fields_num();
        vector<string> get_fields_names();

        string get_field_name(int col);
        int get_field_idx(string& name );

    private:
        MYSQL*      _conn;
        MYSQL_RES*  _res;
        //MYSQL_FIELD*    _field;
        //MYSQL_ROW       _row;
        //int             _num_fields;

};

class CEzMysqlDB
{
    public:
        CEzMysqlDB():query(NULL), _conn(NULL) {}; 

        ~CEzMysqlDB() { disconnect(); }

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
        int replaceRow(const string& tabName, const string& values);

        unsigned long escape_string(char *to, const char *from, unsigned long length);

    public:
        CEzMysqlQ* query;
    private:
        MYSQL* _conn;
};

#endif

