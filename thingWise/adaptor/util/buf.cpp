//
//

#include <string.h>
#include "buf.h"

BOOL Buf::isBE()
{
    union
    {
        unsigned long int i;
        unsigned char s[4];
    }c = {0x12345678};

    return (0x12 == c.s[0]);
}

BOOL Buf::isLE()
{
    union{
        unsigned long bits32;
        unsigned char bytes[4];
    }value;
    value.bytes[0] = 0;
    value.bytes[1] = 1;
    value.bytes[2] = 0;
    value.bytes[3] = 0;

    return value.bits32 == 256;
}

UINT16 Buf::Crc16(char* InStr, UINT32 len)
{
    UINT16 Crc16Table[256];
    UINT32 i,j;
    UINT16 Crc;
    for (i = 0; i < 256; i++){
        Crc = i;
        for (j = 0; j < 8; j++){
            if(Crc & 0x1)
                Crc = (Crc >> 1) ^ 0xA001;
            else
                Crc >>= 1;
        }
        Crc16Table[i] = Crc;
    }

    Crc=0x0000;
    for(i=0; i<len; i++){
        Crc = (Crc >> 8) ^ Crc16Table[(Crc & 0xFF) ^ InStr[i]];

    }

    return Crc;
}

string Buf::toString(char* in, int len, const char* fmt)
{
    int size = 2048;
    char* str = new char [size];
    memset(str, 0, size);

    for (int i = 0; i < len; i++)
    {
        snprintf(str+strlen(str), size-strlen(str), fmt, in[i]);
    }

    string ret(str);
    delete [] str;
    return ret;
}

#define TEST

#ifdef _BUF_TEST

#include <iostream>

int main()
{
    char test[] = {0, 1, 2, 3, 4, 5, 6,7, 8, 9, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    Buf buf(test, 16);
    cout <<  Buf::toString(test, 16, " %.2X") << endl;
    cout << Buf::Crc16(test, 16) << endl;
    cout << buf.ReadUInt32(4) << endl;
}

#endif


//----- EOF -----
