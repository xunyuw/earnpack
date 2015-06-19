#ifndef _DATA_HANDLE_H_
#define _DATA_HANDLE_H_

#include "dev.h"
#pragma pack()
#include "ezMysql.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "logger.h"

using namespace std;

typedef struct {
    int len;
    BYTE data[MAX_IR_LEN];
} IRDATA;

typedef struct
{
    string userID;
    string tagID;
    BYTE   keyID;
    WORD   seqNo;
    BYTE   pktNum;
    BYTE   pktIdx;
    IRDATA data;
} IR_INFO;

class CDataHandle
{
    public:
        explicit CDataHandle():db()
        {
            LOGGER_START(Logger::DEBUG, "/tmp/collector.log");
        }
        ~CDataHandle() {};

        bool connectDB(const string& host, const string& user, const string& pwd, const string& dbName) 
        {
            return (0 == db.connect(host, user, pwd, dbName)); //("127.0.0.1", "root", "ehome321", "ehomeDB")); 
        };

        bool update_map4tagfd(int fd);
        bool recvData(int fd, void* buf, int size);
    private:
        string BYTEArr2str(char in[], int size);
        string tagID2str(const BYTE tagID[]);
        bool tagID2Array(const string strTag, BYTE tagID[] );

        string userID2str(const BYTE userID[]);
        bool userID2Array(const string strUser, BYTE userID[]);

        void swapEndian(WORD& v);    

        vector<string> str_split(const string &s, char delim); 

        unsigned short crc16(char* InStr, unsigned int len);

        bool build_sIR(const vector<string>& args, S_IR* sIR);
        void build_sHead(const S_IR& sIR, S_IR_Head* sHead);
        
        void build_sIRCMD(const S_IR& sIR, S_IR_CMD* sCMD);
        void build_sIRCMD(const S_IR_Packet& ir_pkt, BYTE ack, S_IR_CMD* sCMD);

        void build_sIRKey(const S_IR& sIR, S_IR_Key* sIRKey);

        bool build_ir_pkt(const IR_INFO info, S_IR_Packet* ir_pkt);
        void build_sREP_ACK(const S_IR& sIR, BYTE ack, S_REPORT_ACK* sRepACK);

        bool send2dev(const string& tagID, const void* buf, int size);
        bool send2dev(const int fd, const void* buf, int size);

        bool storeDB(int fd, sDEVICE_REPORT* data);
        bool handle_ir_pkt(int fd, S_IR_Packet* buf);
        bool foward2dev(const string& msg);
        bool handle_ctr_ack(int fd, S_IR_CMD* buf);

        bool get_irinfo(const char* userID, const char* tagID, const BYTE keyID, const BYTE pktIdx, IR_INFO* info);
        void save_user_behavor(const string& user, const string& msg);

    private:
        CEzMysqlDB db;
        map<string, int> tag4fd;
        map<int, string> fd4tag;
};
#endif //_DATA_HANDLE_H_
