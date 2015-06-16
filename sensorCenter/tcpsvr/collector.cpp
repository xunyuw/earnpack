
#include "ev.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory> 
#include <iostream>

#include "config.h"
#include "dataHandle.h"
#include "logger.h"

#define MAXLEN 1024 
#define PORT 5012

using namespace std;

int socket_init();
static int setnonblock(int);
void accept_callback(struct ev_loop * loop, ev_io * w, int revent);
void recv_callback(struct ev_loop * loop, ev_io * w, int revent);
void write_callback(struct ev_loop * loop, ev_io * w, int revent);

std::auto_ptr<CDataHandle> pHandle;

int main(int argc, char ** argv)
{
    int listen;
    ev_io ev_io_watcher;
    listen = socket_init();

    pHandle = std::auto_ptr<CDataHandle>(new CDataHandle);

    std::string host;  
    std::string user;  
    std::string pwd;  
    //std::string logPri;

    const string conf("/etc/ehome/config.ini");   
    try
    {
        Config settings(conf); 
        host = settings.Read<string>("host");
        user = settings.Read<string>("user");
        pwd = settings.Read<string>("passwd");

        //logPri = settings.Read<string>("LogPri");
    }   
    catch(...)
    {
        cout << "config file: " << conf << "NOT exist or invalid settings\n"; 
        return -1;
    }

    if (false == pHandle->connectDB(host, user, pwd, "ehomeDB"))
    {
        std::cout << "failed to connect DB" << endl;
        cout << "host: " << host << ", user: " << user << ", pwd: " << pwd << endl;
        return -1;
    }

    printf("TCP server launched, listen port: %d\n", PORT);
    printf("Database connected, host: %s\n", host.c_str());

    struct ev_loop *loop = EV_DEFAULT;
    ev_io_init(&ev_io_watcher, accept_callback, listen, EV_READ);
    ev_io_start(loop, &ev_io_watcher);
    ev_loop(loop, 0);
}

int socket_init()
{
    struct sockaddr_in svr_addr;
    int listener;
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    int so_reuseaddr = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, 
            sizeof(so_reuseaddr));
    bzero(&svr_addr, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(PORT);
    svr_addr.sin_addr.s_addr =  htons(INADDR_ANY);

    if (bind(listener, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1)
    {
        perror("bind error!\n");
        exit(1);
    }

    if (listen(listener, 1024) == -1)
    {
        perror("listen error!\n");
        exit(1);
    }
    return listener;
}

void accept_callback(EV_P_ ev_io *w, int revents)
{
    int newfd;
    struct sockaddr_in sin;
    socklen_t addrlen = sizeof(sin);
    ev_io* accept_watcher = (ev_io*)malloc(sizeof(ev_io));

    while ((newfd = accept(w->fd, (struct sockaddr *) &sin, &addrlen)) < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            continue;
        else
        {
            printf("accept error.[%s]\n", strerror(errno));
            break;
        }
    }

    setnonblock(newfd);

    ev_io_init(accept_watcher, recv_callback, newfd, EV_READ);
    ev_io_start(loop, accept_watcher);
}

void recv_callback(EV_P_ ev_io * w, int revents)
{
    char buffer[MAXLEN - 1] = {0};
    int ret = 0;

    while (true)
    {
        ret = recv(w -> fd, buffer, MAXLEN, 0);
        if (ret > 0)
        {
            //printf("recv message: %d \n", ret);
            //pHandle->storeDB((sDEVICE_REPORT*) buffer);
            pHandle->recvData(w->fd, buffer, ret);
        }
        else if (ret == 0)
        {
            pHandle->update_map4tagfd(w->fd);
            close(w -> fd);
            ev_io_stop(loop, w);
            free(w);
            return;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
            {
                pHandle->update_map4tagfd(w->fd);
                close(w -> fd);
                ev_io_stop(loop, w);
                free(w);
                return;
            }
        }
        //int fd = w -> fd;
        //ev_io_stop(loop, w);
        //ev_io_init(w, write_callback, fd, EV_WRITE);
        //ev_io_start(loop, w);
        //printf("socket fd: %d, trun read 2 write loop! ", fd);
        return;
    }
}

void write_callback(EV_P_ ev_io * w, int revents)
{
    char buffer[1024] = {0};
    snprintf(buffer, 1023, "try libev server!\n");
    write(w -> fd, buffer, strlen(buffer));
    int fd = w -> fd;
    ev_io_stop(EV_A, w);
    ev_io_init(w, recv_callback, fd, EV_READ);
    ev_io_start(loop, w);
}

static int setnonblock(int fd) 
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags < 0) return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) return -1; 
    return 0;
}

//
//EOF
