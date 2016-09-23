
#pragma once


#include "callback.h"
#include <memory>
#include <list>

namespace mqtt_async {

using namespace std;  

class MqttClient
{
    public:
        explicit MqttClient (const string&  uri, const string& cid):
        monitor_init(false),
        serverURI(uri),
         clientID(cid),
         monitor(this)
        {
            	MQTTAsync_create(&client,  serverURI.c_str(),  clientID.c_str(),  MQTTCLIENT_PERSISTENCE_NONE, NULL);
        }

        ~MqttClient() {
            	MQTTAsync_destroy(&client);
        }

        void OnConnctLost(const ConnLostCB& cb)
        {
            monitor.conn_lost_cb = cb;
        }

        void OnMsgRecv(const MsgRecvCB& cb)
        {
            monitor.msg_recv_cb = cb;
        }

        void OnDelived(const DeliveryDoneCB& cb)
        {
            monitor.delivery_done_cb = cb;
        }

        void SaveToken(std::shared_ptr<Token> stok);
        void RemoveToken(Token* tok);

        bool Connect(int interval,  const ActDoneCB& cb);
        bool Subscribe(string topic, int qos, const ActDoneCB& cb) ;
        bool Publish(const std::string& topic, const void* payload,  INT32 len, int qos,  int retained/*default=0*/, const ActDoneCB& cb) ;   
        bool Disconnect(int tmout, const ActDoneCB& cb) ;

    private:
        bool        monitor_init;
        string      serverURI;
        string	    clientID;
        MQTTAsync client;
        Monitor   monitor;
        mutable std::mutex lock_;
        std::list<std::shared_ptr<Token>>  saved_tokens;

};
}

