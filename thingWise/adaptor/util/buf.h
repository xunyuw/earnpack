//
//

#pragma once

#include "common.h"
#include <arpa/inet.h>
#include <string>

using namespace std;

class Buf
{
    public:
        static BOOL isBE();
        static BOOL isLE();

        static UINT16 SwapEnd16(UINT16 A) {
            return (((static_cast<UINT16>(A) & 0xff00) >> 8) | \
                    ((static_cast<UINT16>(A) & 0x00ff) << 8));
        }

        static UINT32 SwapEnd32(UINT32 A) {
                return  (((static_cast<UINT32>(A) & 0xff000000) >> 24) | \
                ((static_cast<UINT32>(A) & 0x00ff0000) >> 8) | \
                ((static_cast<UINT32>(A) & 0x0000ff00) << 8) | \
                ((static_cast<UINT32>(A) & 0x000000ff) << 24));
        }

        static UINT16 Crc16(char* InStr, UINT32 len);
        static string toString(char* in, int len, const char* fmt);

    public:
        explicit Buf(char* buf, UINT32 len): pBuf(buf), length(len) {}
        ~Buf() {}
        UINT32 GetLen() {return length;}
        char* GetBlock(UINT16 offset) {return const_cast<char*>(pBuf+offset);}

        INT8 ReadInt8(UINT16 offset = 0) { return static_cast<INT8>(pBuf[offset]); }
        INT16 ReadInt16(UINT16 offset = 0) { return static_cast<INT16>(pBuf[offset]); }
        UINT16 ReadUInt16(UINT16 offset = 0) { return static_cast<UINT16>(pBuf[offset]); }
        INT32 ReadInt32(UINT16 offset = 0) { return static_cast<INT32>(pBuf[offset]); }
        UINT32 ReadUInt32(UINT16 offset = 0) { return static_cast<UINT32>(pBuf[offset]); }

    private:
        Buf& operator=(const Buf&) = delete; 
        Buf(const Buf&) = delete; 

    private:
        char*   pBuf;
        UINT32  length;
};

#ifdef SWAP_END
  #define N2HS(V) Buf::SwapEnd16(V)
  #define H2NS(V) Buf::SwapEnd16(V)
#else
  #define N2HS(V) ntohs(V)
  #define H2NS(V) htons(V)
#endif


#if 0
#define BigLittleSwap16(A)        ((((UINT16)(A) & 0xff00) >> 8) | \
                (((UINT16)(A) & 0x00ff) << 8))


#define BigLittleSwap32(A)        ((((UINT32)(A) & 0xff000000) >> 24) | \
                (((UINT32)(A) & 0x00ff0000) >> 8) | \
                (((UINT32)(A) & 0x0000ff00) << 8) | \
                (((UINT32)(A) & 0x000000ff) << 24))


#define H2NL(A)        (Buf::isBE() ? (A) : BigLittleSwap32(A))

#define N2HL(A)        H2NL(A)

#define H2NS(A)        (Buf::isBE() ? (A) : BigLittleSwap16(A))

#define N2HS(A)        H2NS(A)

#endif


// --- EOF ---
