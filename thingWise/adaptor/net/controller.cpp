

#include "controller.h"

bool Controller::Run()
{   
    TRACE("Controller::Run");
    string cid("adaptor");
    string uri = string("tcp://173.39.202.21:1883");
    mqtt_svc.Init( uri, cid);
    int port = 3000;  //get from config
    tcp_svc.Init(port);

    mqtt_svc.Monitor();
    tcp_svc.Monitor();     
    loop.run(); 
    return true;
}


void Controller::RecvTcpData(TcpService* tcp_svc, React::Tcp::Connection* conn, const void* buf, UINT32 size)
{
    TRACE("RecvTcpData");
    TRACE(mqtt_svc.isActive() );
    if ( false == mqtt_svc.isActive() )
       return;

    string topic = string("adaptor/forward");
    string msg = string("{\"act\":\"forward\"}");
    mqtt_svc.Publish(topic, msg);
        //parserData
        // 1.  response to tcp client
        //2.  publish to mqtt 
}

void Controller::ClientDisconnect(TcpService* tcp_svc, React::Tcp::Connection* conn)
{
    //?  
    TRACE("ClientDisconnect");
}

void Controller::RecvMqttData(MqttService* mqtt_svc, mqtt_async::MqttClient* client,  string& topic,  const void* buf,  int size) 
{
    TRACE("RecvMqttData");
}


int main()
{
    Controller ctrl;
    ctrl.Run();
    return 0;
}