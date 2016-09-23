//
//

#pragma once

#include "../util/buf.h"

#pragma pack(push, 1)
namespace GT06GPS {

// 4.3
enum class MsgType {
      UNK=0x00,
      REG=0x01,
      LOC=0x12,
      STA=0x13,
      STR=0x15,
      ALM=0x16,
      Q  =0x1A,
      TR =0x94,
      ANS=0x21,
      RES=0x80
};

//4.1
typedef struct {
    UINT16  start;
    INT8    pktLen;
    INT8    msgT;
}S_HEAD78;
//4.1
typedef struct {
    UINT16  start;
    UINT16  pktLen;
    INT8    msgT;
}S_HEAD79;
//4.0
typedef struct {
    UINT16  seq;
    UINT16  crc;
    UINT16  stop;
} S_TAIL;

//5.1.1
class TID
{
    public:
        string toJson()
        {
            //TODO
            return string("");
        }
    private:
        INT8 IMEI[8];
};

//
//5.2.1.9
typedef struct {
    UINT16 undefined : 2;
    UINT16 north  : 1;
    UINT16 west   : 1;
    UINT16 ready : 1;
    UINT16 rt : 1;
    UINT16 accEnable : 1;
    UINT16 acc : 1;
} S_GPS_ST;

//5.2
class I_GPS
{
    public:
        string toJson(){
            UINT32 count = static_cast<UINT32>(c|0x0F);
            //UINT32 size = static_cast<UINT32>(c|0xF0);
            UINT32 vv = static_cast<UINT32>(v);
            S_GPS_ST* st = reinterpret_cast<S_GPS_ST*>(&d[0]);
            UINT32 dir = static_cast<UINT32>(((d[0]|0x03)<<8)|d[1]);
            //===========
            char ret[128] = {0};
            snprintf(ret, 128, "{"
                    "\"count\":%d,"
                    "\"lat\":%f,"
                    "\"lon\":%f,"
                    "\"speed\": %d,"
                    "\"acc\": %d,"
                    "\"accEnable\": %d,"
                    "\"rt\": %d,"
                    "\"ready\": %d,"
                    "\"west\": %d,"
                    "\"north\":%d,"
                    "\"dir\":  %d,"
                    "}",
                    count,
                    toDegree(x),
                    toDegree(y),
                    vv,
                    st->acc,
                    st->accEnable,
                    st->rt,
                    st->ready,
                    st->west,
                    st->north,
                    dir);
            return string(ret);
        }

    protected:
        double toDegree(INT8 v[]) {
            return static_cast<double>((v[0] << 24 | v[1] << 16 | v[2] << 8 |  \
                                               v[3])/(static_cast<double>(3000 * 60)));
        }

    private:
        INT8 c;
        INT8 x[4];
        INT8 y[4];
        INT8 v;
        INT8 d[2];
};

//5.2
class I_LBS
{
    public:
        string toJson()
        {
            char ret[128] = {0};
            snprintf(ret, 128, "{"
                    "\"MCC\" : %d, "
                    "\"MNC\" : %d, "
                    "\"LAC\" : %d, "
                    "\"CID\" : %d"
                    "}",
                   mcc, //*(static_cast<UINT16*>(mcc)),
                   mnc,
                   lac, //*(static_cast<UINT32*>(lac)),
                   (cid[0] << 16 | cid[1] << 8 | cid[3])
            );
            return string(ret);
        }

    private:
        INT16 mcc;  //5.2.1.10
        INT8 mnc;
        INT16 lac;  //5.2.1.12
        INT8 cid[3];
};

//5.2
class Pkt_LOC {
    public:
        string toJson() {
            return "{ \"T\": " + GetTime() +         \
                     ", \"GPS\": " + gps.toJson() +  \
                     ", \"LBS\": " + lbs.toJson() +  \
                     ", \"route\": " +std::to_string(route) + \
                   " }";
        }

    protected:
        string GetTime()  //"2012-05-17T08:14:15.656Z"
        {
            char szT[32] = {0};
            snprintf(szT, 32, "20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.00Z",
                    t[0], t[1], t[2], t[3], t[4], t[5]);
            return string(t);
        }

    private:
        INT8  t[6];
        I_GPS gps;
        I_LBS lbs;
        INT32  route;//5.2.1.14
};

//5.3.1.14
enum class Alm4T { NORMAL=0X00, SHAKE, POWEROFF, POWERLOW, SOS, UN5, UN6, UN7};
typedef struct {
	UINT16 	fortify     : 1;
	UINT16  acc         : 1;
	UINT16  onCharge    : 1;
	UINT16  alm         : 3;
	UINT16  gpsReady    : 1;
	UINT16  oilPowerOff : 1;
} S_TERM;  //BYTE

enum class PowerLvl {OFF = 0, L1, L2, L3, L4, L5, L6};
enum class SigIntense {OFF = 0, L1, L2, L3, L4};
enum class Alm {
    NORMAL=0,
    SOS,
    POWEROFF,
    SHAKE,
    INSCOPE,
    OUTSCOPE,
    HISPEED,
    MOVE=0X09,
    INBLIND,
    OUTBLIND
};

//5.3.1
enum class Lang{CN=1, EN=2};

typedef struct {
    INT8    alm;
    INT8    lang;
} S_ALM_LANG;

class I_STA
{
    public:
        string toJson()
        {
            return "{ \"term\" : " + GetTerm()               + \
                   ", \"power\" : " + std::to_string(power)    + \
                   ", \"signal\": " + std::to_string(sig)      + \
                   ", \"alarm\" : " + std::to_string(alarm[0]) + \
                   ", \"lang\"  : " + std::to_string(alarm[1]) + \
                "}";
        }
    protected:
        string GetTerm()
        {
            char ret[128] = {0};
            snprintf(ret, 128, "{ "
                "\"fortify\" : %d, "
                "\"acc\" : %d, "
                "\"onCharge\" : %d, "
                "\"alm\" : %d, "
                "\"gpsReady\" : %d, "
                "\"oilPowerOff\" : %d "
                "}",
                i_term.fortify,
                i_term.acc,
                i_term.onCharge,
                i_term.alm,
                i_term.gpsReady,
                i_term.oilPowerOff
            );
            return string(ret);
        }
    private:
        S_TERM  i_term;
        INT8    power;
        INT8    sig;
        INT8    alarm[2];
};

class Pkt_Alm
{
    public:
        string toJson() {
            return "{ \"T\": " + GetTime() +       \
                   ", \"GPS\": " + gps.toJson() +  \
                   ", \"LBS\": " + lbs.toJson() +  \
                   ", \"status\": " +st.toJson() + \
                   " }";
        }

    protected:
        string GetTime()   //"2012-05-17T08:14:15.656Z"
        {
            char szT[32] = {0};
            snprintf(szT, 32, "20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.00Z",
                    t[0], t[1], t[2], t[3], t[4], t[5]);
            return string(szT);
        }
    private:
        INT8  t[6];
        I_GPS gps;
        I_LBS lbs;
        I_STA st;
} ;

class Pkt_Sta
{
    public:
        string toJson()
        {
            return st.toJson();
        }

    private:
        I_STA st;
};

}

#pragma pack(pop)

//
//--------------   EOF   ------------

