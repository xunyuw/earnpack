/* Sample TCP client */
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "../dataHandle.h"

using namespace std;

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;

   if (argc != 2)
   {
      printf("usage:  client <IP address>\n");
      exit(1);
   }

   sockfd=socket(AF_INET,SOCK_STREAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(5012);

   connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    sDEVICE_REPORT* report =  new sDEVICE_REPORT;
    
    report->sSensorInfo.cTagID[0] = 0x11;
    report->sSensorInfo.cTagID[1] = 0x22;
    report->sSensorInfo.cTagID[2] = 0x33;
    report->sSensorInfo.cTagID[3] = 0x44;
    //snprintf(tagID, 16, "%x%x%x%x", tag0, tag1, tag2, tag3);

    //char rssi[12];
    //snprintf(rssi, 12, "%x", report->sSensorInfo.cRSSI);
    report->sSensorInfo.cRSSI = 0xE5;

    //char vList[12];
    //snprintf(vList, 12, "11101100");

    //WORD bat =  ntohs(report->sSensorInfo.wBat);
    report->sSensorInfo.wBat = htons(0x1A2B);

    sSDATA* data = (sSDATA*)report->acSensorData;
//    BYTE tV = data->tV;        //temperature
//    WORD hV = ntohs(data->hV); //humidity
//    WORD fV = ntohs(data->fV); //formaldehyde
//    WORD pV = ntohs(data->pV); //pm
//    WORD oV = ntohs(data->oV); //tvoc

    data->tV = htons(25);        //temperature
    data->hV = htons(234); //humidity
    data->fV = htons(345); //formaldehyde
    data->pV = htons(456); //pm
    data->oV = htons(567); //tvoc

    int num = 0;
    if((num=send(sockfd, report, sizeof(sDEVICE_REPORT),0))==-1){
        printf("send() error\n");
        delete report;
        exit(1);
    }
    cout << num << endl;
    delete report;
    close(sockfd);
    return 0;
/*
   while (fgets(sendline, 10000,stdin) != NULL)
   {
      sendto(sockfd,sendline,strlen(sendline),0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
      recvline[n]=0;
      fputs(recvline,stdout);
   }
*/
}
