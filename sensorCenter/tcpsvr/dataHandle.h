#ifndef _DATA_HANDLE_H_
#define _DATA_HANDLE_H_

#include "ezMysql.h"

#pragma pack(1)

using namespace std;

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef unsigned int   UINT;

typedef struct {
    BYTE   cVersion;
    WORD   wPackCRC;
    WORD   wLength;
}sPACK_HEAD;

typedef struct
{
    BYTE cCMD;
    BYTE cTagID[4];
    BYTE cSensorFlag;
    BYTE cRSSI;
    WORD wBat;
    WORD wPackNo;
    BYTE cCnt;
}sSENSOR_INFO;

typedef struct
{
    sPACK_HEAD   sPackHead;
    sSENSOR_INFO sSensorInfo;
    BYTE         acReserved[6];
    BYTE         acSensorData[32];
}sDEVICE_REPORT;

typedef struct
{
    //BYTE tV;
    //BYTE unUsed;
    WORD tV;
    WORD hV;
    WORD fV;
    WORD pV;
    WORD oV;
} sSDATA;

class CDataHandle
{
    public:
        explicit CDataHandle():db() {};
        ~CDataHandle() {};
        bool connectDB(const string& host, const string& user, const string& pwd, const string& dbName) 
        {
            return (0 == db.connect(host, user, pwd, dbName)); //("127.0.0.1", "root", "ehome321", "ehomeDB")); 
        };
       
        void hello() {cout << "hello :-)" << endl; };
        bool storeDB(sDEVICE_REPORT* data);
    private:
        CEzMysqlDB db;
};
#endif //_DATA_HANDLE_H_
