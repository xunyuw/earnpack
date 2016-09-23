/*******************************************************************************
 * Copyright (c) 2012, 2013 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

extern "C" {
    #include "stdio.h"
    #include "stdlib.h"

    #include "../../src/MQTTClient.h"
}

#include <unistd.h>   
#include <string.h>
#include "config.h"

using namespace std;

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

int count = 0;

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    printf("Message arrived:\n");
    printf("   topic: %s\n", topicName);
    printf("   message: (len: %d), rev count: %d\n", message->payloadlen, 
    count++);
    /*
    payloadptr = (char*)message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    */
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
/*
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1

*/
int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
    string host;
    string topic;
    int qos;

    char szCID[32] = {0};
    snprintf(szCID, 32, "sub-%d", getpid());
    string cid(szCID);

    const string conf("/etc/mqttc/config_sub.ini");
    try
    {
        Config settings(conf);
        host = settings.Read<string>("host");
        //cid = settings.Read<string>("cid");
        topic = settings.Read<string>("topic");
        qos = settings.Read<int>("qos");  
    }
    catch(...)
    {
        cout << "config file: " << conf << "NOT exist or invalid settings\n";
        return -1;
    }

    //std::cout << argc << argv[1][0] << endl; 
    if (argc == 2 && (argv[1][0] >= '0' && argv[1][0] <= '2'))
        qos = argv[1][0] - '0';
        
    string address;
    address = "tcp://" + host +":1883";

    MQTTClient_create(&client, address.c_str(), cid.c_str(),
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS:%d\n\n"
           "Press Q<Enter> to quit\n\n", topic.c_str(), cid.c_str(), qos);
    MQTTClient_subscribe(client, topic.c_str(), qos);

    do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
