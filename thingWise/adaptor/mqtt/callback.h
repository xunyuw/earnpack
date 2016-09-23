//
#pragma once

extern "C" {
    #include  "MQTTAsync.h"
}

#include "../util/common.h"
#include "type.h"


namespace mqtt_async{
   using namespace std;
   
    class MqttClient;
    class Monitor
    {
        public:
            explicit Monitor(MqttClient* c): client(c) {}
            ~Monitor() {}
            
        public:
            static void ConnectionLost(void *context, char *cause) ;
            static int MsgRecv(void* context, char* topicName, int topicLen,  MQTTAsync_message* msg);
            static void Delivered(void* context, MQTTAsync_token token);

        private:
            MqttClient*  client;
            ConnLostCB  conn_lost_cb;
            MsgRecvCB   msg_recv_cb;
            DeliveryDoneCB  delivery_done_cb;
            friend class MqttClient;
    };


    //class ActCallback
    class Token
    {
        public:
            explicit Token(MqttClient* c): client(c) {}
            ~Token() {}        

        public:
            //void MQTTAsync_onSuccess(void* context, MQTTAsync_successData* response);
            static void OnSuccess(void* context, MQTTAsync_successData* resp);
            static void OnFailed (void* context, MQTTAsync_failureData* resp);
            MqttClient* getClient()  { return client; }          

        private:
            MqttClient*  client;
            ActDoneCB  act_done_cb;
            friend class MqttClient;
    };
}

