
#pragma once


#include <iostream>
#include <reactcpp.h>

#include "../devices/gt06gps_msg.h"
#include "../util/common.h"
#include "../mqtt/mqtt.h"

class Controller;

class Service
{
    public:
        explicit Service(React::MainLoop* _loop, Controller*  _ctrl):
                            loop(_loop), 
                            ctrl(_ctrl),
                            actived(false)  {



                            }

        virtual ~Service() {}

        virtual void  Monitor() {};


        std::shared_ptr<React::IntervalWatcher>  StartTimer(double t, const React::IntervalCallback& cb)
        {
            //std::shared_ptr<React::TimeoutWatcher>  timer;
            if (!loop)
                return nullptr;

            return loop->onInterval(0.5, t, cb);
        }

        void Actived(bool  act){ actived = act; }
        bool isActive() {return actived; }

    protected:
        React::MainLoop*   loop;
        Controller*   ctrl;    

    private:    
        bool    actived;    
};


class TcpService: public Service
{
    public:
        //explicit Service(std::shared_ptr<React::Tcp::Connection> conn): _conn(conn){}
        explicit TcpService(React::MainLoop* _loop, Controller*  _ctrl):
                    Service(_loop, _ctrl),
                    tcpSvr(nullptr){

                     }  

       virtual ~TcpService() { if (tcpSvr)  delete tcpSvr;}  

        void  Init(int port)
        {
            tcpSvr = new React::Tcp::Server(loop, port);
            Actived(true);
        }

        virtual void  Monitor();

    protected:
        void RecvData(React::Tcp::Connection* conn,  const void* buf, UINT32 size);
        void Disconnect(React::Tcp::Connection* conn);


    private:
        React::Tcp::Server* tcpSvr;
};

class MqttService: public Service
{
    public:
        //explicit Service(std::shared_ptr<React::Tcp::Connection> conn): _conn(conn){}
        explicit MqttService(React::MainLoop* _loop, Controller*  _ctrl):      
                    Service(_loop,  _ctrl), 
                    mc(nullptr),
                    keepAliveT(120) {

                     }

        ~MqttService() { if (mc) delete mc; }

        void Init(string& uri, string& cid) 
        {
            mc = new mqtt_async::MqttClient(uri, cid);
        }

        virtual void  Monitor() ;
        
   
        void Publish(const string& topic, const string& msg)
        {
                 mqtt_async::ActDoneCB cb;
                 //Publish(const std::string& topic, const void* payload,  INT32 len, int qos,  int retained/*default=0*/, const ActDoneCB& cb) ; 
                 TRACE("Publish");
                 TRACE(topic);
                 TRACE(msg);
                 if (mc)
                    mc->Publish(topic,  msg.c_str(),  msg.length(), 0,  0, cb);
        }

    protected:
        void RecvData(mqtt_async::MqttClient* client,  string& topic,  const void*  payload, UINT32 size);

    private:
        mqtt_async::MqttClient*  mc;
        int  keepAliveT;
};
