
#ifndef _DEV_H_
#define _DEV_H_

#pragma pack(1)

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
    BYTE         acSensorData[10];
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


////////////////////////////////////////////////////////////////////////////////////////


#define PREDEFINE_ID                0x05   // ID prefix of air device

#define AIR_PROTOCOL                0x81   // protocol version
#define CMD_DATA_REPORT             0x90   // device report status to server
#define CMD_DATA_REPORT_ACK         0x91   // server ack to device for status report
#define CMD_IR_STUDY                0xA0   // enter IR study state(KEY_ID exist,but not used)
#define CMD_IR_KEY_STUDY            0xA1   // IR study command (include KEY_ID)
#define CMD_IR_UPLOAD_PACK          0xA2   // device upload IR packet to server
#define CMD_IR_ACK_SERVER           0xA3   // server ack to device for split packet upload
#define CMD_IR_DONE                 0xA4   // exit IR study state
#define CMD_IR_KEY_CONTROL          0xA5   // IR Control command(include KEY_ID)
#define CMD_IR_DOWN_PACK            0xA6   // server send IR packet to device
#define CMD_IR_ACK_DEVICE           0xA7   // device ack to server for split packet download

//#define MAX_IR_LEN                  100 //(128 - 23 - 5)
#define MAX_IR_LEN                  96 //(124 - 23 - 5)
#define USER_ID_LEN                 12

typedef struct {
    BYTE Version;
    WORD PackCRC;
    WORD Length;
}S_IR_Head;

typedef struct 
{
    S_IR_Head sHead;
    BYTE CMD; 
    BYTE TagID[4];
    BYTE ACK;
    BYTE KeyID;
    WORD SeqNo;
    BYTE PackIndex;
    BYTE PackNumber; 
}S_REPORT_ACK; 


typedef struct 
{
    BYTE CMD; 
    BYTE TagID[4];
    BYTE ACK;
    BYTE KeyID;
    WORD SeqNo;
    BYTE UserID[USER_ID_LEN];
    BYTE PackIndex;
    BYTE PackNumber; 
}S_IR; 


typedef struct 
{
    S_IR_Head  sHeader;
    S_IR sIR;
}S_IR_CMD;

typedef struct 
{
    S_IR_Head  sHeader;
    S_IR sIR;
    BYTE  KeyID;
}S_IR_Key;

typedef struct 
{
    S_IR_Head  sHeader;
    S_IR  sIR;
    BYTE  acIRData[MAX_IR_LEN];
}S_IR_Packet;


#endif
