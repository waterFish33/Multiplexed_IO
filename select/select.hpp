#pragma once
#include <sys/select.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include "dictionaries.hpp"

#define NUMS (sizeof(fd_set) * 8)
using namespace std;
class Select
{
public:
    Select() {}
    Select(int port) : _port(port)
    {
    }

    void Init()
    {
        dictionary.init();
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);

        // 填写addr字段信息
        _socketaddr.sin_addr.s_addr = INADDR_ANY;
        _socketaddr.sin_family = AF_INET;
        _socketaddr.sin_port = htons(_port);
        // 绑定
        bind(_listensockfd, (struct sockaddr *)&_socketaddr, sizeof(_socketaddr));

        for (int i = 0; i < NUMS; i++)
        {
            //cout << NUMS << endl;
            //cout << i << endl;
            fd_array[i] = -1;
        }
    }
    void Listen()
    {
        // 监听
        listen(_listensockfd, 5);
        fd_array[0] = _listensockfd;
    }
    void AcceptSocket()
    {
        for (int s = 0; s < NUMS; s++)
        {
            if (fd_array[s] == -1)
            {
                sockaddr_in peer;
                socklen_t len = sizeof(peer);
                fd_array[s] = accept(_listensockfd, (struct sockaddr *)&peer, &len);
                break;
            }
        }
    }
    void Server()
    {
       

        while (1)
        {
            FD_ZERO(&readfd);
            maxfd = 0;
            //构建readfd
            for (int i = 0; i < NUMS; i++)        
            {
                if (fd_array[i] != -1)
                {
                    cout<<fd_array[i]<<" ";
                    FD_SET(fd_array[i], &readfd);
                    if (fd_array[i] > maxfd)
                    {
                        maxfd = fd_array[i];
                    }
                }
            }
            cout<<endl;
            select(maxfd + 1, &readfd, nullptr, nullptr, NULL);
            cout<<"select"<<endl;
            //for循环找到已经准备就绪的文件描述符
            for (int i = 0; i < NUMS; i++)
            {
              // cout<<i<<endl;
                if (FD_ISSET(fd_array[i], &readfd))
                {
                    cout<<fd_array[i]<<":";
                    if (fd_array[i] == _listensockfd)
                    { 
                        // 监听到新链接的到来
                        AcceptSocket();
                    }
                    else
                    { 
                        // 正常的读写
                        char buf[1024];
                        size_t size = recv(fd_array[i], buf, sizeof(buf) , 0);
                        if (size > 0)
                        {
                            //把输入字符串的"\n"替换成"\0"

                            buf[size-1] = '\0';
                            cout<<buf<<endl;
                            string chinese = dictionary.find(buf);
                            send(fd_array[i], chinese.c_str(), chinese.size(), 0);
                        }
                        else
                        {
                            cout<<"close fd"<<endl;
                            close(fd_array[i]);
                            fd_array[i] = -1;
                        }
                    }
                }
                
            }
        }
    }
    ~Select() {
        close(_listensockfd);
        
    }

private:
    int _listensockfd;
    int maxfd;
    sockaddr_in _socketaddr;
    int _port;
    int fd_array[NUMS];
    dictionaries dictionary;
     fd_set readfd;
};