
#include "callback.h"
#include "mqtt.h"

void mqtt_async::Monitor::ConnectionLost(void *context, char *cause) 
{
    if (context) {
        Monitor* m = static_cast<Monitor*>(context);
        if  (m->conn_lost_cb)
        {
            string strCause = cause ? std::string(cause) : std::string();
            m->conn_lost_cb(m->client, strCause);
        }
    }
}

int mqtt_async::Monitor::MsgRecv(void* context, char* topicName, int topicLen,  MQTTAsync_message* msg)
{
    if (context) {
        Monitor* m = static_cast<Monitor*>(context);
        if (m->msg_recv_cb)
        {
            std::string topic(topicName,  topicLen);
            m->msg_recv_cb(m->client, topic,  msg->payload, msg->payloadlen);
        }
    }
    MQTTAsync_freeMessage(&msg);
    MQTTAsync_free(topicName);

    // TODO: Should the user code determine the return value?
    // The Java version does doesn't seem to...
    return (!0);
}        

void mqtt_async::Monitor::Delivered(void* context, MQTTAsync_token token)
{
    if (context) {
        Monitor* m = static_cast<Monitor*>(context);
        if (m->delivery_done_cb)
            m->delivery_done_cb(m->client, static_cast<int>(token));
    }           
}

void mqtt_async::Token::OnSuccess(void* context, MQTTAsync_successData* resp)
{
    if (context) {
        Token* t = static_cast<Token*>(context);
        t->client->RemoveToken(t);
        if (t->act_done_cb)
            t->act_done_cb(true, t->client,  resp? resp->token: 0);
    }             
}

void mqtt_async::Token::OnFailed (void* context, MQTTAsync_failureData* resp)
{
    if (context) {
        Token* t = static_cast<Token*>(context);
        t->getClient()->RemoveToken(t);
        if (t->act_done_cb)
            t->act_done_cb(false, t->client,  resp? resp->token: 0);
    }               
}


