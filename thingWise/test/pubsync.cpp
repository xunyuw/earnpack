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
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "12345678"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    string host;
    string topic;
    int qos;
    int size;
    int interval;

    char szCID[32] = {0};
    snprintf(szCID, 32, "pub-%d", getpid());
    string cid(szCID);
    

    const string conf("/etc/mqttc/config_pub.ini");
    try
    {
        Config settings(conf);
        host = settings.Read<string>("host");
        //cid = settings.Read<string>("cid");
        topic = settings.Read<string>("topic");
        qos = settings.Read<int>("qos"); 
        size = settings.Read<int>("size"); 
        interval = settings.Read<int>("interval");
        if (size < 0 || size > 256) 
            size = 256;
    }
    catch(...)
    {
        cout << "config file: " << conf << "NOT exist or invalid settings\n";
        return -1;
    }

    if (argc == 2 && (argv[1][0] >= '0' && argv[1][0] <= '2'))
        qos = argv[1][0] - '0';    


    string address;
    address = "tcp://" + host +":1883";
      
    string str;
    str.append(size, 'a');
    
    
    pubmsg.payload = const_cast<char*>(str.c_str());
    pubmsg.payloadlen = size;
    pubmsg.qos = qos;
    pubmsg.retained = 0;


    MQTTClient_create(&client, address.c_str(), cid.c_str(),
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    
    printf("publish to topic %s\nfor client %s using QoS:%d\n\n"
           "once per %dms\n", topic.c_str(), cid.c_str(), qos, interval);

    int count = 0;
    do
    {
        MQTTClient_publishMessage(client, topic.c_str(), &pubmsg, &token);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        usleep(1000*interval);
    } while(1);


    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
