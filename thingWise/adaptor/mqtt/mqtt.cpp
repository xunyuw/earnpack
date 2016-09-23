

#include "../util/common.h"
#include "mqtt.h"


void  mqtt_async::MqttClient::SaveToken(std::shared_ptr<Token> stok)
{
	if (stok) {
		guard g(lock_);
		saved_tokens.push_back(stok);
	}    
}

void mqtt_async::MqttClient::RemoveToken(Token* tok)
{
	if (!tok)
		return;

	guard g(lock_);
	for (auto p=saved_tokens.begin(); p!=saved_tokens.end(); ++p) {
		if (p->get() == tok) {
			saved_tokens.erase(p);
			return;
		}
	}
}

bool mqtt_async::MqttClient::Connect(int interval,  const ActDoneCB& cb) 
{
    int rc;
    if (!monitor_init)
    {
        MQTTAsync_connectionLost* cl = monitor.conn_lost_cb? &Monitor::ConnectionLost : nullptr;
        MQTTAsync_messageArrived* ma = monitor.msg_recv_cb? &Monitor::MsgRecv : nullptr;
        MQTTAsync_deliveryComplete* dc =  monitor.delivery_done_cb ?  &Monitor::Delivered : nullptr;         
        rc = MQTTAsync_setCallbacks(client,  &monitor,  cl, ma, dc);
        if (rc != MQTTASYNC_SUCCESS)
            throw rc;    
        monitor_init = true;          
    }


    MQTTAsync_connectOptions opts = MQTTAsync_connectOptions_initializer;
    opts.keepAliveInterval = interval;
    opts.cleansession = 1;

    Token* tok = nullptr;
    std::shared_ptr<Token> stok;
    if (cb)
    {
        tok = new Token(this);
        stok = std::shared_ptr<Token>(tok);
        stok->act_done_cb = cb;
        SaveToken(stok);
    }

    opts.onSuccess = cb?&Token::OnSuccess : nullptr ;
    opts.onFailure =   cb?&Token::OnFailed : nullptr ;
    opts.context = tok;
    
    if ((rc = MQTTAsync_connect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        if (tok)
            RemoveToken(tok);
        throw rc;
    }      
    return true;     
}

bool mqtt_async::MqttClient::Subscribe(string topic, int qos, const ActDoneCB& cb) 
{
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;    
    Token* tok = nullptr;
    std::shared_ptr<Token> stok;
    if (cb)
    {
        tok = new Token(this);
        stok = std::shared_ptr<Token>(tok);
        stok->act_done_cb = cb;
        SaveToken(stok);
    }

    opts.onSuccess = cb?&Token::OnSuccess : nullptr ;
    opts.onFailure =   cb?&Token::OnFailed : nullptr ;
    opts.context = tok;
    int rc;
    if ((rc = MQTTAsync_subscribe(client,  topic.c_str(),  qos, &opts)) != MQTTASYNC_SUCCESS)
    {
        if (tok)
            RemoveToken(tok);
        return false;
    }       
    return true;     
}

bool mqtt_async::MqttClient::Publish(const std::string& topic, const void* payload,  INT32 len, int qos,  int retained/*default=0*/, const ActDoneCB& cb)    
{
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    Token* tok = nullptr;
    std::shared_ptr<Token> stok;
    TRACE("mqtt_async::MqttClient::Publish");
    if (cb)
    {
        TRACE("mqtt_async::MqttClient::Publish  with  callback");
        tok = new Token(this);
        stok = std::shared_ptr<Token>(tok);
        stok->act_done_cb = cb;
        SaveToken(stok);
    }

    opts.onSuccess = cb?&Token::OnSuccess : nullptr ;
    opts.onFailure =   cb?&Token::OnFailed : nullptr ;
    opts.context = tok;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    pubmsg.payload = const_cast<void*>(payload);
    pubmsg.payloadlen =  len;
    pubmsg.qos = qos; 
    pubmsg.retained = retained;

    int rc = MQTTAsync_sendMessage(client, (char*)topic.c_str(), &pubmsg, &opts) ;
     if (rc != MQTTASYNC_SUCCESS)  
     {
         if (tok)
            RemoveToken(tok);

        TRACE("mqtt_async::MqttClient::Publish   return false");    
        return false;
     }  
     return true;    
}   

bool mqtt_async::MqttClient::Disconnect(int tmout, const ActDoneCB& cb) 
{
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    Token* tok =  nullptr;
    std::shared_ptr<Token> stok;
    if (cb)
    {
        tok = new Token(this);
        stok = std::shared_ptr<Token>(tok);
        stok->act_done_cb = cb;
        SaveToken(stok);
    }

    opts.onSuccess = cb?&Token::OnSuccess : nullptr ;
    opts.onFailure =   cb?&Token::OnFailed : nullptr ;
    opts.timeout = tmout;
    opts.context = tok;

    int rc;
    if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        if (tok)
            RemoveToken(tok);
        throw rc;
    }        
    return true;    
}



