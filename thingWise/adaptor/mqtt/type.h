#pragma once

#include "../util/common.h"
#include <iostream>
#include <string>
#include <functional>
#include <exception>
#include <mutex>

namespace mqtt_async
{
    class MqttClient;

    //(void *context, char *cause) 
    using   ConnLostCB = std::function<void(MqttClient*,  std::string&)>;  
    // topic,  msg->payload, msg->payloadlen); 
    using   MsgRecvCB  = std::function<void(MqttClient*,  std::string&,  const void*,  UINT32)>;
    //void MQTTAsync_deliveryComplete(void* context, MQTTAsync_token token)
    //mqttclient, token
    using   DeliveryDoneCB =  std::function<void(MqttClient*, INT32)>;   

    //void MQTTAsync_onSuccess(void* context, MQTTAsync_successData* response);
    // mqttclient, token
    using ActDoneCB = std::function<void(bool, MqttClient*, INT32)>; 

    typedef std::unique_lock<std::mutex> guard;
}