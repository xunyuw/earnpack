#include <cstdio>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include "dataHandle.h"
#include "logger.h"

#include <iterator>
#include "string.h"

#include "unistd.h"
#include <stdlib.h> 
#include <time.h> 

string CDataHandle::tagID2str(const BYTE tagID[])
{
    char szTag[16] = {0};
    snprintf(szTag, 16, "%X%.2X%.2X%.2X", 
            tagID[0], tagID[1], tagID[2], tagID[3]);
    return string(szTag);
}

bool CDataHandle::tagID2Array(const string strTag, BYTE tagID[] )
{
    int len = strTag.size();
    int offset = 0;
    if (len == 7 )
    {
        offset = 1;
        tagID[0] = static_cast<BYTE>(stoi(strTag.substr(0, 1), nullptr, 16));
    }
    else if (len == 8)
    {
        offset = 2;
        tagID[0] = static_cast<BYTE>(stoi(strTag.substr(0, 2), nullptr, 16));
    }
    else
        return false;


    for (int i = 0; i < 3;  i++)
    {
        tagID[i+1] = static_cast<BYTE>(stoi(strTag.substr(offset+i*2, 2), nullptr, 16));
    }
    return true;
}

string CDataHandle::userID2str(const BYTE userID[])
{
    char strUser[USER_ID_LEN]={0};
    for (int i = 0; i < USER_ID_LEN -1; i++)
        strUser[i] = static_cast<char>((BYTE)userID[i] + 48);

    return string(strUser);
}

bool CDataHandle::userID2Array(const string strUser, BYTE userID[])
{
    if (strUser.size() != (USER_ID_LEN -1) )
        return false;

    const char* p = strUser.c_str();
    for (int i = 0; i < USER_ID_LEN-1; i++)
        userID[i] = static_cast<BYTE>((char)p[i] - 48);

    userID[USER_ID_LEN-1] = 0;
    return true;
}

void CDataHandle::swapEndian(WORD& v)    
{    
    BYTE* p = (BYTE*)&v;    
    BYTE t = *(p);    
    *(p) = *(p+1);    
    *(p+1) = t;    
}

#if 1
#define SWAP_END(V) swapEndian(V)
#else
#define SWAP_END(V) 
#endif
vector<string> CDataHandle::str_split(const string &s, char delim) 
{
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) 
    {
        tokens.push_back(item);
    }
    return tokens;
}

unsigned short CDataHandle::crc16(char* InStr, unsigned int len)
{     
  unsigned short Crc16Table[256];   
  unsigned int i,j;  
  unsigned short Crc;     
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

void CDataHandle::build_sHead(const S_IR& sIR, S_IR_Head* sHead)
{
    memset(sHead, 0, sizeof(S_IR_Head));
    sHead->Version = (BYTE)AIR_PROTOCOL;
    WORD len = static_cast<WORD>(sizeof(S_IR));
    sHead->Length = htons(len);
    SWAP_END(sHead->Length);
    WORD crc = static_cast<WORD>(crc16((char*)&sIR, len));
    sHead->PackCRC = htons(crc);
}

bool CDataHandle::build_sIR(const vector<string>& args, S_IR* sIR)
{
    memset(sIR, 0, sizeof(S_IR));

    sIR->CMD = static_cast<BYTE>(stoi(args[0], nullptr, 16));

    // userID
    if (11 != args[1].size())
        return false;

    for (int k = 0; k < 11; k++ )
    {
        sIR->UserID[k] = static_cast<BYTE>((char)args[1][k] - 48);
    }
    sIR->UserID[11] = (BYTE)0;

    if (false == tagID2Array(args[2], sIR->TagID))
        return false;

    sIR->KeyID = static_cast<BYTE>(stoi(args[3], nullptr, 10));
    WORD seq = static_cast<WORD>(stoi(args[4], nullptr, 10));
    sIR->SeqNo = htons(seq);
    SWAP_END(sIR->SeqNo);
    sIR->PackIndex = (BYTE)1;
    sIR->PackNumber= (BYTE)1;

#if 0
    int j = 0;
    cout << "CMD: " << (int)sIR->CMD << "\n";
    cout << "userID: \n";
    for (j = 0; j < 12; j++)
        cout << (int)sIR->UserID[j] << "\n";
    cout << "tagid: \n";
    for (j = 0; j < 4; j++)
        cout << (int)sIR->TagID[j] << "\n";
#endif

    return true;
}

void CDataHandle::build_sIRCMD(const S_IR& sIR, S_IR_CMD* sCMD)
{
    S_IR_Head* sHead = (S_IR_Head*)&(sCMD->sHeader);
    memset(sHead, 0, sizeof(S_IR_Head));
    sHead->Version = (BYTE)AIR_PROTOCOL;
    WORD len = static_cast<WORD>(sizeof(S_IR));
    sHead->Length = htons(len);
    SWAP_END(sHead->Length);
    WORD crc = static_cast<WORD>(crc16((char*)&sIR, len));
    sHead->PackCRC = htons(crc);

    memcpy((S_IR*)&(sCMD->sIR), (S_IR*)&sIR, sizeof(S_IR));
}

//for ir_pkt ack, cmd=A3
void CDataHandle::build_sIRCMD(const S_IR_Packet& ir_pkt, BYTE ack, S_IR_CMD* sCMD)
{
    S_IR sIR;
    memcpy((S_IR*)&sIR, (S_IR*)&(ir_pkt.sIR), sizeof(S_IR));
    sIR.CMD = static_cast<BYTE>(0xA3);
    sIR.ACK = ack;
    build_sIRCMD(sIR, sCMD);
}

void CDataHandle::build_sIRKey(const S_IR& sIR, S_IR_Key* sIRKey)
{
    S_IR_Head* sHead = (S_IR_Head*)&(sIRKey->sHeader);
    memset(sHead, 0, sizeof(S_IR_Head));
    sHead->Version = (BYTE)AIR_PROTOCOL;
    WORD len = static_cast<WORD>(sizeof(S_IR_Key));
    sHead->Length = htons(len);
    SWAP_END(sHead->Length);
    WORD crc = static_cast<WORD>(crc16((char*)&sIR, len));
    sHead->PackCRC = htons(crc);

    memcpy((S_IR*)&(sIRKey->sIR), (S_IR*)&sIR, sizeof(S_IR));
    sIRKey->KeyID = sIR.KeyID;
}

bool CDataHandle::build_ir_pkt(const IR_INFO info, S_IR_Packet* ir_pkt)
{
    S_IR* psIR = (S_IR*)&(ir_pkt->sIR);
    psIR->CMD = 0xA6;
    if (false == tagID2Array(info.tagID, psIR->TagID))
        return false;
    psIR->ACK = 0;
    psIR->KeyID = info.keyID;
    psIR->SeqNo = info.seqNo;
    SWAP_END(psIR->SeqNo);
    if (false == userID2Array(info.userID, psIR->UserID))
        return false;

#if 1
    printf("userid: \n");
    for(int i = 0; i < 12; i++ )
        printf("%d", (BYTE)(psIR->UserID[i]));
    printf("\n");

    printf("tagid: \n");
    for(int i = 0; i < 4; i++ )
        printf("0x%X", (BYTE)(psIR->TagID[i]));
    printf("\n");

#endif
    psIR->PackIndex = info.pktIdx;
    psIR->PackNumber = info.pktNum;

    if (info.data.len > MAX_IR_LEN)
        return false;

    memcpy(ir_pkt->acIRData, (char*)info.data.data, info.data.len);
#if 1
    printf("send ir data (keyID:%d, index: %d): \n",psIR->KeyID, psIR->PackIndex);
    for(int i = 0; i < 10; i++)
        printf("%d ", (BYTE)ir_pkt->acIRData[i]);
    printf("\n");
#endif

    S_IR_Head* ph;
    ph = (S_IR_Head*)&(ir_pkt->sHeader);
    ph->Version =  (BYTE)AIR_PROTOCOL;
    ph->PackCRC = (WORD)0;
    WORD len = static_cast<WORD>(info.data.len + sizeof(S_IR));
    ph->Length = htons(len);
    SWAP_END(ph->Length);
    return true;
}

void CDataHandle::build_sREP_ACK(const S_IR& sIR, BYTE ack, S_REPORT_ACK* sRepACK)
{
   // S_REPORT_ACK sRepACK;
    sRepACK->sHead.Version =  (BYTE)AIR_PROTOCOL;
    sRepACK->sHead.PackCRC = (WORD)0;
    //sRepACK->sHead.Length = (WORD)(sizeof(S_REPORT_ACK)-sizeof(S_IR_Head));
    sRepACK->sHead.Length = (WORD)(sizeof(S_REPORT_ACK));
    SWAP_END(sRepACK->sHead.Length);
    sRepACK->CMD = (BYTE)0xA3;
    for (int i = 0; i<4; i++)
        sRepACK->TagID[i] = sIR.TagID[i];
    sRepACK->ACK = ack;
    sRepACK->KeyID = sIR.KeyID;
    sRepACK->SeqNo = sIR.SeqNo;
    SWAP_END(sRepACK->SeqNo);
    sRepACK->PackIndex = sIR.PackIndex;
    sRepACK->PackNumber = sIR.PackNumber;        
}

bool CDataHandle::send2dev(const int fd, const void* buf, int size)
{
    LOGGER_WRITE(Logger::DEBUG, "Entry send2dev...")
    int res = write(fd, buf, size);
    LOGGER_WRITE(Logger::DEBUG, "send2dev fd: %d, size:%d", fd, res)
    return (res > 0);
}

bool CDataHandle::send2dev(const string& tagID, const void* buf, int size)
{
    LOGGER_WRITE(Logger::DEBUG, "Entry send2dev...")
    int fd;
    map<string, int>::iterator iter = tag4fd.find(tagID);
    if(iter != tag4fd.end())
        fd = iter->second;
    else
    {
        LOGGER_WRITE(Logger::WARNING, "Not found the fd correspond to tagID: %s", tagID.c_str());
        return false;
    }
    int res = write(fd, buf, size);
    LOGGER_WRITE(Logger::DEBUG, "send2dev fd: %d, size:%d", fd, res)
    return (res > 0);
}

bool CDataHandle::update_map4tagfd(int fd)
{
    LOGGER_WRITE(Logger::DEBUG, "Entry update_map4tagfd...")
    map<int, string>::iterator iter = fd4tag.find(fd);
    if(iter != fd4tag.end())
    {
        LOGGER_WRITE(Logger::INFO, "remove fd: %d", fd);
        string tag = iter->second;
        fd4tag.erase(fd);
        map<string, int>::iterator it = tag4fd.find(tag);
        if(it != tag4fd.end())
            tag4fd.erase(tag);

        return true;
    }
    return false;
}


bool CDataHandle::get_irinfo(const char* userID, const char* tagID, const BYTE keyID, const BYTE pktIdx, IR_INFO* info)
{
    CEzMysqlQ* q = db.query;
    q->execute("select * from irInfo where UserID='%s' and TagID='%s' and KeyID=%d and pktIdx=%d", 
            userID, tagID, keyID, pktIdx);
    char** row = q->fetchone();
    if (row == NULL)
        return false;

    info->userID = string(row[0]);
    info->tagID = string(row[1]);
    info->keyID = static_cast<BYTE>(atoi(row[2]));
    info->seqNo = static_cast<WORD>(atoi(row[3])); 
    info->pktNum = static_cast<BYTE>(atoi(row[4]));
    info->pktIdx = static_cast<BYTE>(atoi(row[5]));
    memcpy((char*)&(info->data), row[6], q->get_ele_len(6));
    q->close();
    return true;
}

#if 1
bool CDataHandle::foward2dev(const string& msg)
{
    //A0&13501897143&549E7FD&1&0
    LOGGER_WRITE(Logger::DEBUG, "Entry forward2dev...")
    vector<string> args = str_split(msg, '&');
    if (args.size() != 6 )
    {
        LOGGER_WRITE(Logger::DEBUG, "unknown msg: %s", msg.c_str());
        return false;
    }
   // std::copy(args.begin(), args.end(), ostream_iterator<string>(std::cout, "\n")); 

    if (0 == args[0].compare("A0") || 0 == args[0].compare("A4"))
    {
        S_IR_CMD  sCMD;
        S_IR      sIR;
        if(false == build_sIR(args, &sIR))
            return false;
        build_sIRCMD(sIR, &sCMD);
        return send2dev(args[2], (char*)&sCMD, sizeof(S_IR_CMD));
    }
    else if (0 == args[0].compare("A1"))
    {
        S_IR_Key sIRKey;
        S_IR      sIR;
        if(false == build_sIR(args, &sIR))
            return false;
        build_sIRKey(sIR, &sIRKey);
        return send2dev(args[2], (char*)&sIRKey, sizeof(S_IR_Key));
    }
    else if ( 0 == args[0].compare("A6"))
    {
        LOGGER_WRITE(Logger::DEBUG, "send ctr pkt to dev")
        string userID = args[1];
        string tagID = args[2];
        BYTE keyID = static_cast<BYTE>(stoi(args[3], nullptr, 10));

        LOGGER_WRITE(Logger::DEBUG, "userID: %s, tagID: %s, keyID: %d", 
            userID.c_str(), tagID.c_str(), keyID);

        IR_INFO info;
        bool ret = get_irinfo(userID.c_str(), tagID.c_str(), keyID, 1, (IR_INFO*)&info);
        if (false == ret)
            return false;
        S_IR_Packet ir_pkt;
        if (false == build_ir_pkt(info,  &ir_pkt))
            return false;

        LOGGER_WRITE(Logger::DEBUG, "data-len: %d, sizeof(ir_pkt):%d", info.data.len,sizeof(ir_pkt));
        int size = ir_pkt.sHeader.Length+5;
        return send2dev(args[2], (char*)&ir_pkt, size);

    }
    else
        LOGGER_WRITE(Logger::DEBUG, "unsupported msg: %s", msg.c_str());


    return true;
}
#else

bool CDataHandle::foward2dev(const string& msg)
{
    //A0&13501897143&549E7FD&1&0
    //LOGGER_WRITE(Logger::DEBUG, "Entry forward2dev...")
    vector<string> args = str_split(msg, '&');
    if (args.size() != 6 )
        return false;
   // std::copy(args.begin(), args.end(), ostream_iterator<string>(std::cout, "\n")); 

    if (0 == args[0].compare("A0") || 0 == args[0].compare("A4"))
    {
        S_IR_CMD  sCMD;
        S_IR      sIR;
        if(false == build_sIR(args, &sIR))
            return false;
        build_sIRCMD(sIR, &sCMD);
        send2dev(args[2], (char*)&sCMD, sizeof(S_IR_CMD));
    }
    if (0 == args[0].compare("A0"))
    {
        args[0] = "A1";
        S_IR_Key sIRKey;
        S_IR      sIR;
        if(false == build_sIR(args, &sIR))
            return false;
        build_sIRKey(sIR, &sIRKey);
        return send2dev(args[2], (char*)&sIRKey, sizeof(S_IR_Key));
    }

    return true;
}
#endif

bool CDataHandle::storeDB(int fd, sDEVICE_REPORT* report)
{
    //LOGGER_WRITE(Logger::DEBUG, "Entry storeDB...")

    string strTag = tagID2str((BYTE*)report->sSensorInfo.cTagID);
    tag4fd[strTag] = fd; //------
    fd4tag[fd] = strTag;

    char rssi[12];
    snprintf(rssi, 12, "%X", (BYTE)report->sSensorInfo.cRSSI);

    char vList[12];
    snprintf(vList, 12, "11101100");

    WORD bat =  ntohs((WORD)report->sSensorInfo.wBat);

    sSDATA* data = (sSDATA*)report->acSensorData;
    //BYTE tV = (BYTE)data->tV;        //temperature
    WORD tV = ntohs((WORD)data->tV);        //temperature
    WORD hV = ntohs((WORD)data->hV); //humidity
    WORD fV = ntohs((WORD)data->fV); //formaldehyde
    WORD pV = ntohs((WORD)data->pV); //pm
    WORD oV = ntohs((WORD)data->oV); //tvoc

    /*
    `TagID` varchar(16) NOT NULL,
    `RSSI`  varchar(16) DEFAULT '',
    `vList` varchar(16) DEFAULT '',
    `Bat`  smallint DEFAULT '0',
    `oV`   smallint DEFAULT '0',
    `tV`   smallint DEFAULT '0',
    `hV`   smallint DEFAULT '0',
    `pV`   smallint DEFAULT '0',
    `fV`   smallint DEFAULT '0',
    `rT`   TIMESTAMP not null
    */

    char szV[200];
    snprintf(szV, 200, "(\"%s\", \"%s\", \"%s\", %d, %d, %d, %d, %d, %d, NOW())",
                        strTag.c_str(), rssi, vList,
                        bat, oV, tV, hV, pV, fV ); 
    //cout << szV << endl;

    return (0 == db.insertRow("sensorDatum",string(szV)));
}   

bool CDataHandle::handle_ir_pkt(int fd, S_IR_Packet* buf)
{
    const S_IR* const sIR = (S_IR*)&(buf->sIR); 
    const S_IR_Head* const sHeader = (S_IR_Head*)&(buf->sHeader); 
    //const S_IR_Head* const sHeader = (S_IR_Head*)&(buf->sHeader); 
#if 0
    if (sHeader->Version !=  (BYTE)AIR_PROTOCOL)
    {
        LOGGER_WRITE(Logger::ERROR, "invalid packet, Version error: 0x%X", sHeader->Version);
        return false;
    }

    if (sIR->CMD != 0xA2)
    {
        LOGGER_WRITE(Logger::ERROR, "sIR->CMD(%X) must be 0xA2", sIR->CMD);
        return false;
    }
#endif 
    
    LOGGER_WRITE(Logger::DEBUG, "handle_ir_pk recv:\n \
            len: %d, \n \
            KeyID: %d, SeqNo: %d; \n \
            PackIndex:%d,PackNumber:%d", 
            sHeader->Length, 
            sIR->KeyID, sIR->SeqNo,
            sIR->PackIndex, sIR->PackNumber);
    // TODO


    char to[256] = {0};
    IRDATA ir;
    ir.len = sHeader->Length - sizeof(S_IR);
    memcpy((BYTE*)(ir.data), (BYTE*)(buf->acIRData), ir.len);
    int len = db.escape_string(to, (char*)&ir, sizeof(IRDATA));
    if (len <= 0) // avoid compile warning
        return false;

    char userID[12] = {0};
    for (int i = 0; i < 11; i++)
        userID[i]=static_cast<char>((BYTE)(buf->sIR.UserID[i])+48);

    LOGGER_WRITE(Logger::DEBUG, "userID: %s, data-len: %d", userID, ir.len);

#if 1
    printf("recv ir data (keyID:%d, index: %d): \n", sIR->KeyID, sIR->PackIndex);
    for(int i = 0; i < 10; i++)
        printf("%d ", (BYTE)(buf->acIRData[i]));
    printf("\n");
#endif

    CEzMysqlQ* q = db.query;
    string strTag = tagID2str((BYTE*)buf->sIR.TagID);
    if (sIR->PackIndex == 1)
        q->execute("delete from irInfo where UserID='%s' and TagID='%s' and KeyID=%d",
                userID, strTag.c_str(), sIR->KeyID); 
    q->execute("insert into irInfo values('%s', '%s', %d, %d, %d, %d, '%s') ",
                userID, strTag.c_str(), sIR->KeyID, sIR->SeqNo, 
                sIR->PackNumber, sIR->PackIndex, to);
    //

    S_IR_CMD sACK;
    build_sIRCMD(*buf, 0, (S_IR_CMD*)&sACK);
    return send2dev(fd, (char*)&sACK, sizeof(S_IR_CMD));
    /*
    S_REPORT_ACK sRepACK;
    build_sREP_ACK(*sIR, 1, (S_REPORT_ACK*)&sRepACK);
    LOGGER_WRITE(Logger::DEBUG, "send::PackIndex:%d,PackNumber:%d", 
        sRepACK.PackIndex,sRepACK.PackNumber);
    return send2dev(fd, (char*)&sRepACK, sizeof(S_REPORT_ACK));
    */
}

bool CDataHandle::handle_ctr_ack(int fd, S_IR_CMD* buf)
{
    S_IR* psIR = (S_IR*)&(buf->sIR);
    LOGGER_WRITE(Logger::DEBUG, "Entry handle_ctr_ack");
    string tagID = tagID2str(psIR->TagID);
    string userID = userID2str(psIR->UserID);
    BYTE keyID = psIR->KeyID;
    BYTE ack = psIR->ACK;
    BYTE idx = psIR->PackIndex;
    BYTE num = psIR->PackNumber;

    BYTE next;
    if (ack  == 0)
        next = idx + 1;
    else
        next = idx;

    if  (next > num)
        return false;

    LOGGER_WRITE(Logger::DEBUG, "userID: %s, tagID: %s, keyID: %d, idx: %d, num: %d, next:%d", 
            userID.c_str(), tagID.c_str(), keyID, idx, num, next);

    IR_INFO info;
    bool ret = get_irinfo(userID.c_str(), tagID.c_str(), keyID, next, (IR_INFO*)&info);
    if (false == ret)
        return false;
    S_IR_Packet ir_pkt;
    if (false == build_ir_pkt(info,  &ir_pkt))
        return false;

    int size = ir_pkt.sHeader.Length+5;
    return send2dev(fd, (char*)&ir_pkt, size);
}

bool CDataHandle::recvData(int fd, void* buf, const int size)
{
    S_IR_CMD* p = (S_IR_CMD*)buf;
    if ((p->sHeader.Version ==  (BYTE)AIR_PROTOCOL)
        || (p->sHeader.Version ==  (BYTE)0x82))
    {
        switch (p->sIR.CMD)
        {
            case 0xA2:
                handle_ir_pkt(fd, (S_IR_Packet*)buf);
                break;
            case 0xA7:
                handle_ctr_ack(fd, (S_IR_CMD*)buf);
                break;
            case 0x90:
                storeDB(fd, (sDEVICE_REPORT*)buf);
                break;
            default:
                LOGGER_WRITE(Logger::WARNING, "invalid pkt, fd=%d, size=%d, CMD: 0x%X", 
                        fd, size, p->sIR.CMD);
                break;
        }
    }
    else
    {
        //const int sz_report = sizeof(sDEVICE_REPORT);
        const int sz_fwd_msg = 99;

        switch(size)
        {
            case sz_fwd_msg:
                LOGGER_WRITE(Logger::INFO, "recv forward msg, fd=%d, size=%d", fd, size);
                foward2dev(string((char*)buf));
                break;
            default:
                LOGGER_WRITE(Logger::WARNING, "unknown msg, fd=%d, size=%d", fd, size);
                break;
        }
    }
    return true;
}

#if 0
bool CDataHandle::recvData(int fd, void* buf, const int size)
{
    const int sz_report = sizeof(sDEVICE_REPORT);
    const int sz_ir_pkt = sizeof(S_IR_Packet);
    const int sz_rep_ack = sizeof(S_REPORT_ACK);

    const int sz_ir_key = sizeof(S_IR_Key);
    const int sz_ir_cmd = sizeof(S_IR_CMD);

    //LOGGER_WRITE(Logger::DEBUG, "Entry recvData: fd=%d, size=%d", fd, size);
    switch(size)
    {
        case sz_report:
            storeDB(fd, (sDEVICE_REPORT*)buf);
            break;
        case sz_ir_pkt:
            handle_ir_pkt(fd, (S_IR_Packet*)buf);
            break;
        case sz_rep_ack:
            LOGGER_WRITE(Logger::INFO, "recv ir report ack, fd=%d", fd);
            break;
        case sz_ir_key:
            LOGGER_WRITE(Logger::INFO, "recv ir key, fd=%d", fd);
            break;
        case sz_ir_cmd:
            LOGGER_WRITE(Logger::INFO, "recv ir cmd, fd=%d, size=%d", fd, size);
            break;
        case 99:  //forward msg
            LOGGER_WRITE(Logger::INFO, "recv forward msg, fd=%d, size=%d", fd, size);
            foward2dev(string((char*)buf));
            break;
        default:
            if (size >= 50)
                handle_ir_pkt(fd, (S_IR_Packet*)buf);
            else
                LOGGER_WRITE(Logger::WARNING, "unknown msg, fd=%d, size=%d", fd, size);
            break;
    }

    return true;
}
#endif 


//
//// EOF
