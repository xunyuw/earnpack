#pragma once


#include "service.h"


class Controller
{
    public:
        explicit Controller() : tcp_svc(&loop, this), mqtt_svc(&loop, this) {}
        ~Controller() {}

        bool Run();

        void RecvTcpData(TcpService* tcp_svc, React::Tcp::Connection* conn,  const void* buf, UINT32 size);
        void ClientDisconnect(TcpService* tcp_svc, React::Tcp::Connection* conn);
        void RecvMqttData(MqttService* mqtt_svc, mqtt_async::MqttClient* client,  string& topic,  const void* buf,  int size);

    private:
        React::MainLoop loop;
        TcpService  tcp_svc;
        MqttService mqtt_svc;
};