

#include "../util/logger.h"
#include "../util/config.h"
#include "service.h"
#include "controller.h"
#include "../mqtt/type.h"

void TcpService::RecvData(React::Tcp::Connection* conn,  const void* buf, UINT32 size)
{
    if (ctrl) ctrl->RecvTcpData(this, conn, buf, size);
} 

void TcpService::Disconnect(React::Tcp::Connection* conn)
{
    if (ctrl) ctrl->ClientDisconnect(this, conn);
}

void  TcpService::Monitor()
{

    loop->onSignal(SIGINT, [this]() -> bool {  
        delete this->tcpSvr;
        TRACE("SIGNAL");
        exit(0) ; 
        return true; 
    });


    tcpSvr->onConnect([this]()->bool{
        /*
        auto conn = std::make_shared<React::Tcp::Connection>(server);
        auto pConn = conn.get();
        cout << "new connect\n";
        auto in = std::make_shared<React::Tcp::In<1024>>(pConn);
        */
        auto pConn = new React::Tcp::Connection( (React::Tcp::Server&)*(this->tcpSvr));
        TRACE("new incoming connction");
        auto in = new React::Tcp::In<1024>(pConn);

        in->onData([pConn, this](const void *buf, size_t size)->bool{
                this->RecvData(pConn,  buf, size);
                return true;
            });

        in->onLost([pConn, in, this]()-> void {
                pConn->close();
                this->Disconnect(pConn);
                delete  in;
                delete  pConn;
            });

        return true;
    });            
}

void MqttService::RecvData(mqtt_async::MqttClient* client,  string& topic,  const void*  payload, UINT32 size)
{
        if (ctrl) ctrl->RecvMqttData(this,  client,  topic, payload, size);
}

void MqttService::Monitor()
{
    if (!mc)
        return;

    TRACE("MqttService::Monitor");
    mc->OnConnctLost([this] (mqtt_async::MqttClient* client,  std::string& cause) -> void{
        TRACE("OnConnctLost: ");
        #ifdef TRACE
        if (!cause.empty())
            TRACE(cause);
       #endif

       this->Actived(false);
       this->StartTimer(3.0, [this, client]()->bool {
            bool ret;    
            client->Connect(this->keepAliveT, [this, &ret](bool connected, mqtt_async::MqttClient* cli, INT32 token)->void {
                TRACE("Connect?: ");
                TRACE(connected);
                ret = connected;
                this->Actived(connected);
            } );
            return !ret;
       });
    });

    mc->OnMsgRecv([](mqtt_async::MqttClient* client, std::string& topic,  const void* payload,   UINT32 size) -> void {
        TRACE("OnMsgRecv");
         //this->RecvData(client,  topic, payload, size);
    });

    mc->Connect(keepAliveT, [this](bool connected, mqtt_async::MqttClient* client, INT32 token)->void {
        TRACE("Connect: ");
        TRACE(connected);
        this->Actived(connected);
        if (!connected)
            throw runtime_error("failed to connect broker");
    } );
    
    [this]() -> void{this->Actived(true); } ();
    //actived = true;
}