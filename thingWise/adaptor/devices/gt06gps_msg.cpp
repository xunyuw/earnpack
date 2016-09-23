//
//
//

#include "gt06gps_msg.h"

namespace GT06GPS {


BOOL Message::Parse()
{
    /*
     * HEADER:| start 2  | pkt len 1/2 | protocol num 1
     * INFO  :  info content N
     * TAIL  :  seq 2    | crc 2       |  stop 2 |
     * pktLen  = [protocol num  ---> crc]
     * crc blk = [pkt len ----> seq]
     */
    UINT16 offset = 0;
    UINT16 start = N2HS(ReadUInt16());
    if (start == 0x7878)
    {
        S_HEAD78* p = reinterpret_cast<S_HEAD78*>(pHead);
        info.msgT = static_cast<MsgType>(p->msgT);
        info.size = p->pktLen - 5;
        offset = 4;
    }
    else if (start == 0x7979)
    {
        S_HEAD79* p = reinterpret_cast<S_HEAD79*>(pHead);
        info.msgT = static_cast<MsgType>(p->msgT);
        info.size = p->pktLen - 5;
        offset = 5;
    }
    else
        return FALSE;

    info.offset = offset;
    if ((info.msgT != MsgType::REG) &&
            (info.msgT != MsgType::LOC) &&
            (info.msgT != MsgType::STA) &&
            (info.msgT != MsgType::STR) &&
            (info.msgT != MsgType::ALM) &&
            (info.msgT != MsgType::Q) &&
            (info.msgT != MsgType::TR) &&
            (info.msgT != MsgType::ANS) &&
            (info.msgT != MsgType::RES))
        return FALSE;

    info.p = GetBlock(offset);

    pTail = reinterpret_cast<S_TAIL*>(GetBlock(offset + info.size));

    if (GetLen() != static_cast<UINT32>(offset + info.size + 6))
        return FALSE;

    if (N2HS(pTail->stop) != 0x0D0A )
            return FALSE;

    UINT16 calc = Buf::Crc16(GetBlock(2), (offset + info.size)); // (offset - 2 + info.size + 2));
    if (N2HS(pTail->crc) != calc)
        return FALSE;

    return TRUE;
}

string Message::GetInfo()
{
    string strInfo;
    MsgType msgT;
    switch(info.msgT)
    {
        case MsgType::REG:
            strInfo = Buf::toString(info.p, info.size, "%.2X");
            msgT = info.msgT;
            break;
            
        case MsgType::LOC:
        {
            Pkt_LOC* p = (Pkt_LOC*)info.p;
            strInfo = p->toJson();
            msgT = info.msgT;
            break;
        }
            
        case MsgType::ALM:
        {
            Pkt_Alm* p = (Pkt_Alm*)info.p;
            strInfo = p->toJson();
            msgT = info.msgT;
            break;
        }    
        case MsgType::STA:
        {
            Pkt_Sta* p = (Pkt_Sta*)info.p;
            strInfo = p->toJson();
            msgT = info.msgT;
            break;
        }
        default:
            msgT = MsgType::UNK;
            strInfo = "";
            break;
    }

    return strInfo;
}

#if 0
class PktGt06: public Buf
{
    public:
        explicit PktGt06(void* buf, UINT16 len):
            Buf(buf, len),
            msgT(MsgType::UNK),
            pInfo(nullptr),
            N(0),
            seq(0)
            {}

        ~PktGt06() {}

        BOOL Parse()
        {
            UINT16 start = N2HS(ReadUInt16());
            INT8 offset = 2;
            UINT16 calc = 0;
            if (start == 0x7878)
            {
                pktLen = ReadUInt8(offset);
                calc = Buf::Crc16(GetBlock(offset), (pktLen - 2 + 1));
                offset += 1;
            }
            else if (start == 0x7979)
            {
                pktLen = N2HS(ReadUInt16(offset));
                calc = Buf::Crc16(GetBlock(offset), (pktLen - 2 + 2));
                offset += 2;
            }
            else
                return FALSE;

            if (GetLen() != (offset + pktLen + 2))
                return FALSE;

            N = pktLen - 5;
            // check stop bit
            if(0x0D0A != N2HS(ReadUint16(offset + pktLen)))
                return FALSE;

            //check CRC
            if (calc != N2HS(ReadUint16(offset + pktLen - 2)) )
                return FALSE;

            msgT = static_cast<MsgType>(ReadUInt8(offset));
            offset += 1;
            pInfo = GetBlock(offset);
            offset += N;
            seq = ReadUInt16(offset);
            //offset += 2;
            /*
            UINT16 crc = N2HS(ReadUInt16(offset));
            offset += 2;
            UINT16 stop = N2HS(ReadUInt16(offset));
            if (stop != 0x0D0A)
                return FALSE;
            */
        }


        MsgType GetType() {return msgT;}

    private:
        char*    pInfo;
        UINT16   N;
        INT8     seq;
};


#endif

}

//
//--------------   EOF   ------------

