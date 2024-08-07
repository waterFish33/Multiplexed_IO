#pragma once 
#include "dictionaries.hpp"
#include <unistd.h>
#include <poll.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/epoll.h>
#include <string>
using namespace std;
class epoll_server
{
public:
    epoll_server(int port) : _port(port) {}
    void Init()
    {
        _listenfd = socket(AF_INET, SOCK_STREAM, 0);
        localaddr.sin_family = AF_INET;
        localaddr.sin_addr.s_addr = INADDR_ANY;
        localaddr.sin_port = htons(_port);

        bind(_listenfd, (struct sockaddr *)&localaddr, sizeof(localaddr));

        dictionary.Init();

        _epfd = epoll_create(1);
    }
    void Listen()
    {
        listen(_listenfd, 5);
        Add(_listenfd);
    }
    void Add(int socketfd){
        epoll_event event;
        event.data.fd=socketfd;
        event.events=EPOLLIN;
        if(epoll_ctl(_epfd,EPOLL_CTL_ADD,socketfd,&event)){
            perror("epoll_ctl add fail");
        }
    }
    void Delete(int socketfd){
        epoll_ctl(_epfd,EPOLL_CTL_DEL,socketfd,NULL);

    }
    void Start()
    {
        epoll_event events[1000];
        while (1)
        {
            int nfds = epoll_wait(_epfd, events, sizeof(events) / sizeof(events[0]), -1);
            for (int i = 0; i < nfds ; i++)
            {
                if(events[i].data.fd==_listenfd){//有新的链接到来
                    int clientfd=accept(_listenfd, NULL, NULL);
                    Add(clientfd);
                }else{
                    char buf[1024];
                    int size=recv(events[i].data.fd,buf,sizeof(buf)-1,0);
                    buf[size-1]='\0';
                    if(size>0){
                        cout<<"client send:"<<buf<<endl;
                        string chinese=dictionary.Find(buf);
                        send(events[i].data.fd,chinese.c_str(),chinese.size(),0);
                    }else{
                        close(events[i].data.fd);
                        Delete(events[i].data.fd);
                    }
                }
            }
        }

       
    }

    ~epoll_server() {
        close(_listenfd);
    }

private:
    int _listenfd;
    sockaddr_in localaddr;
    int _port;
    int _epfd;
    dictionaries dictionary;
};