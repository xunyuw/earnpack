//
//
#include <iostream>
#include "stdio.h" 

#include "ezMysql.h"

CEzMysqlDB::CEzMysqlDB()
    :conn_(NULL)
{
}  

CEzMysqlDB::~CEzMysqlDB()
{
    disconnect();
}

int CEzMysqlDB::connect(const string& host, 
                        const string& user,
                        const string& passwd,
                        const string& db,
                        const unsigned int port) 
{
    conn_ = mysql_init(NULL);
    if (NULL == conn_)
        return -1;

    string cmd = "CREATE DATABASE IF NOT EXISTS " + db;    
    MYSQL* newC = mysql_real_connect(conn_, 
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
        printf("Error %u: %s\n", mysql_errno(conn_), mysql_error(conn_));
        return -3;
    }
    return 0;    
}

void CEzMysqlDB::disconnect()
{
    if ( conn_ != NULL )
    {
        mysql_close( conn_ );
        conn_ = NULL;
    }
}

int CEzMysqlDB::startTrans()
{
    if (NULL == conn_)
        return -1;

    string cmd = "START TRANSACTION";    
    return mysql_real_query(conn_, cmd.c_str(), cmd.size());
}

int CEzMysqlDB::rollback()
{
    if (NULL == conn_)
        return -1;

    string cmd = "ROLLBACK";    
    return mysql_real_query(conn_, cmd.c_str(), cmd.size());
}

int CEzMysqlDB::commit()
{
    if (NULL == conn_)
        return -1;

    string cmd = "COMMIT";    
    return mysql_real_query(conn_, cmd.c_str(), cmd.size());
}

int CEzMysqlDB::createTable(const string& tabName, const string& schema )
{
    if (NULL == conn_)
        return -1;

    string cmd("create table if not exists ");
    cmd = cmd + tabName + schema + " CHARSET=utf8";
    //create table if not exists tb1(id int(10),t int(3))
    return mysql_real_query(conn_, cmd.c_str(), cmd.size());
}



int CEzMysqlDB::insertRow(const string& tabName, const string& values)
{
    if (NULL == conn_)
        return -1;

    string cmd("INSERT INTO ");
    cmd = cmd + tabName + " VALUES" + values;
    //cout << cmd << endl;
    //mysql_query(conn, "INSERT INTO writers VALUES('Leo Tolstoy')")
    return mysql_real_query(conn_, cmd.c_str(), cmd.size());  
}


#if 0 //BT
#include <assert.h>
int main()
{
    CEzMysqlDB db;
    assert(0 == db.connect("127.0.0.1","xouyang", "123456", "etltable"));
    assert(0 == db.createTable("sensors", "(tagid CHAR(10), time TIMESTAMP not null, tmp int)"));
    assert(0 == db.insertRow("sensors", "(\"12345ab\", NOW(), 30 )"));
    db.disconnect();
    return 0;
}

#endif
