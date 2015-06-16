//
//
#include <iostream>
#include "stdio.h" 
#include <vector>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "ezMysql.h"

#if 0
bool CEzMysqlQ::execute(const string& sql)
{
    if (_conn == NULL)
        return false;

    if ( mysql_real_query(_conn, sql.c_str(), sql.size() ) )
        return false;

    MYSQL_RES *result;
    result = mysql_store_result( _conn );
    if (result) // select
    {   
        _res = result;
        return true;
    }
    else  // no select
        return (0 == mysql_errno(_conn));
}

bool CEzMysqlQ::execute(const char* const sql)
{
    if (_conn == NULL)
        return false;

    if ( mysql_real_query(_conn, sql,(unsigned int)(strlen(sql) )) )
        return false;

    MYSQL_RES *result;
    result = mysql_store_result( _conn );
    if (result) // select
    {   
        _res = result;
        return true;
    }
    else  // no select
        return (0 == mysql_errno(_conn));
    
}

#endif

bool CEzMysqlQ::execute( const char* fmt, ...)    
{
    const int SIZE_OF_BUF=2048;     
    if (_conn == NULL)
        return false;
       
    va_list ap;     
    static char buf[SIZE_OF_BUF] = {0};     
    char *bptr = buf;     
       
    va_start(ap, fmt);     
    vsprintf(bptr, fmt, ap);     
    va_end(ap);  

    if ( mysql_real_query(_conn, buf, (unsigned int)(strlen(buf) )) )
        return false;

    MYSQL_RES *result;
    result = mysql_store_result( _conn );
    if (result) // select
    {   
        _res = result;
        return true;
    }
    else  // no select
        return (0 == mysql_errno(_conn));
}


//for special row
unsigned long CEzMysqlQ::get_ele_len(int idx)
{
    if (idx >= get_fields_num())
        return 0;
    unsigned long *lengths = mysql_fetch_lengths(_res);
    return lengths[idx];
}

MYSQL_ROW CEzMysqlQ::fetchone()
{
    return mysql_fetch_row(_res);
}

vector<MYSQL_ROW> CEzMysqlQ::fetchall()
{
    vector<MYSQL_ROW> ret;
    if (_res == NULL)
        return ret;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(_res)))
    {
        ret.push_back(row);
    }
    return ret;
}

int CEzMysqlQ::get_fields_num()
{
    if (NULL == _res)
        return 0;
    return mysql_num_fields(_res);
}

vector<string> CEzMysqlQ::get_fields_names()
{
    MYSQL_FIELD *field;
    vector<string>  ret;
    if (NULL == _res)
        return ret;

    while((field = mysql_fetch_field(_res)))
    {
        ret.push_back(string(field->name));
    }
    return ret;
}

string CEzMysqlQ::get_field_name(int idx) 
{
    if ( _res == NULL )
        return string("");
    if (idx >=  get_fields_num())
        return string("");

    MYSQL_FIELD* field;
    mysql_field_seek(_res, idx);
    field = mysql_fetch_field(_res);

    if ( field != NULL )
        return string(field->name);
    else
        return string("");
}

int CEzMysqlQ::get_field_idx(string& name )
{
    if ( NULL == _res )
        return -1;

    mysql_field_seek(_res, 0);
    MYSQL_FIELD* field;
 
    int i = 0;
    while(( field = mysql_fetch_field( _res ) ))
    {
        if ( field != NULL && strcmp(field->name, name.c_str()) == 0 )
            return i;
        
        i++;
    }
    return -1;    
}


////////////////////////////////////////////////////////////////////////
//


int CEzMysqlDB::connect(const string& host, 
                        const string& user,
                        const string& passwd,
                        const string& db,
                        const unsigned int port) 
{
    _conn = mysql_init(NULL);
    if (NULL == _conn)
        return -1;

    string cmd = "CREATE DATABASE IF NOT EXISTS " + db;    
    MYSQL* newC = mysql_real_connect(_conn, 
                                host.c_str(),
                                user.c_str(), 
                                passwd.c_str(),
                                db.c_str(),
                                port, 
                                NULL, 
                                0);
    if (NULL == newC)
    {
        disconnect();
        printf("Error %u: %s\n", mysql_errno(_conn), mysql_error(_conn));
        return -3;
    }
    query = new CEzMysqlQ(_conn);
    return 0;    
}

void CEzMysqlDB::disconnect()
{
    if (query != NULL)
    {
        delete query;
        query = NULL;
    }
    if ( _conn != NULL )
    {
        mysql_close( _conn );
        _conn = NULL;
    }
}

int CEzMysqlDB::startTrans()
{
    if (NULL == _conn)
        return -1;

    string cmd = "START TRANSACTION";    
    return mysql_real_query(_conn, cmd.c_str(), cmd.size());
}

int CEzMysqlDB::rollback()
{
    if (NULL == _conn)
        return -1;

    string cmd = "ROLLBACK";    
    return mysql_real_query(_conn, cmd.c_str(), cmd.size());
}

int CEzMysqlDB::commit()
{
    if (NULL == _conn)
        return -1;

    string cmd = "COMMIT";    
    return mysql_real_query(_conn, cmd.c_str(), cmd.size());
}

int CEzMysqlDB::createTable(const string& tabName, const string& schema )
{
    if (NULL == _conn)
        return -1;

    string cmd("create table if not exists ");
    cmd = cmd + tabName + schema + " CHARSET=utf8";
    //create table if not exists tb1(id int(10),t int(3))
    return mysql_real_query(_conn, cmd.c_str(), cmd.size());
}

unsigned long CEzMysqlDB::escape_string(char *to, const char *from, unsigned long length)
{
    if (to == NULL || from == NULL)
        return 0;
    return mysql_real_escape_string(_conn, to, from, length);
}

int CEzMysqlDB::insertRow(const string& tabName, const string& values)
{
    if (NULL == _conn)
        return -1;

    string cmd("INSERT INTO ");
    cmd = cmd + tabName + " VALUES" + values;
    //cout << cmd << "\n";
    return mysql_real_query(_conn, cmd.c_str(), cmd.size());  
}



#if 1 //BT

typedef struct {
    char* tagID;
    char* time;
    char*   v;
}ele;

#include <assert.h>

void ut()
{
    CEzMysqlDB db;
    assert(0 == db.connect("127.0.0.1","root", "ciscooyx", "ehomeDB"));
    CEzMysqlQ* q = db.query;
    const char v[] = {0x01, 0x02, 0xa1, 0xb2};
    char to[128];
    cout << db.escape_string(to, v, 4) << endl;
    q->execute("delete from irInfo where UserID='18616808128' and TagID='11223344' and KeyID=1");
assert(true == q->execute("insert into irInfo values('18616808128', '11223344', 2, 102, 6, 1, '%s' ) ",
                 to  ));

    assert(true == q->execute("select * from irInfo"));
    vector<char**> ret = q->fetchall();
    if (ret.size() > 0)
    {
        cout << "hello" << endl;
        cout << ret[0][0] <<  endl;
        memcpy(to, ret[0][6], 4);
        printf("0x%X, 0x%X\n", (char)to[0], (char)to[3]);
    }
    else
        cout << "no element" << endl;
}

int ut2()
{
    CEzMysqlDB db;
    assert(0 == db.connect("127.0.0.1","root", "ciscooyx", "testDB"));
    assert(0 == db.createTable("sensors", "(tagid CHAR(10), time TIMESTAMP not null, tmp int)"));
    assert(0 == db.insertRow("sensors", "(\"12345ab\", NOW(), 30 )"));
    CEzMysqlQ* q = db.query;
    assert(true == q->execute("select * from sensors"));
    vector<char**> ret = q->fetchall();
    if (ret.size() > 0)
        cout <<  ret[0][0] << "  " << ret[0][1] << " " << sizeof(ret[0][2])<< endl;
    ele* p = (ele*)ret[0];
    cout <<p->tagID << " " << p->time << (p->v) << endl;
    q->close();
    cout << ret.size() << endl;
    db.disconnect();
    return 0;
}

int main()
{
    ut();
    return 0;
}

#endif
