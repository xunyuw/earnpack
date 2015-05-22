#include <cstdio>
#include <arpa/inet.h>
#include <iostream>
#include "dataHandle.h"

using namespace std;

bool CDataHandle::storeDB(sDEVICE_REPORT* report)
{
    char tagID[16] = {0};

    BYTE tag0 = (BYTE)report->sSensorInfo.cTagID[0];
    BYTE tag1 = (BYTE)report->sSensorInfo.cTagID[1];
    BYTE tag2 = (BYTE)report->sSensorInfo.cTagID[2];
    BYTE tag3 = (BYTE)report->sSensorInfo.cTagID[3];
    snprintf(tagID, 16, "%X%X%X%X", tag0, tag1, tag2, tag3);
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
                        tagID, rssi, vList,
                        bat, oV, tV, hV, pV, fV ); 
    //cout << szV << endl;

    return (0 == db.insertRow("sensorDatum",string(szV)));
}    
