//
//
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#pragma once

#include "gt06gps_data.h"
#include "../util/buf.h"

using namespace std;
namespace GT06GPS {
    
typedef struct
{
    MsgType msgT;
    UINT16 offset;
    UINT16 size;
    char*  p;
} S_INFO;

class Message: public Buf
{
    public:
        explicit Message(char* buf, UINT32 len):
            Buf(buf, len),
            pHead(buf),
            pTail(nullptr) {}

        ~Message() {}

        BOOL Parse();
        string GetInfo();
        MsgType GetType() { return info.msgT; }


    private:
        char*   pHead;
        //UINT16  offset;
        S_INFO  info;
        //UINT16  N;
        //char*   pInfo;
        S_TAIL* pTail;
};

}

#endif


//### ----EOF---

