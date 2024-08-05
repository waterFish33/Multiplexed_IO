#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;
int main(int argc, char **argv)
{
    int port = stoi(argv[2]);
    char *ip = argv[1];
    sockaddr_in _addr;
    int _sockfd;
    int wait = 0;
    while (1)
    {
        memset(&_addr, 0, sizeof(_addr));
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(port);
        _addr.sin_addr.s_addr = (inet_addr(ip));
        while (connect(_sockfd, (struct sockaddr *)&_addr, sizeof(_addr)) != 0)
        {
            if (wait == 5)
            {
                return 1;
            }
            perror("connect fail!\n");
            wait++;
            sleep(1);
        }

        cout << "connect server success" << endl;
        cout << "please enter: ";
        char buf[1024];
        fgets(buf, sizeof(buf), stdin);
        // cout<<"my fgets:"<<buf;
        int n = write(_sockfd, buf, strlen(buf));
        if (n > 0)
        {
            int size = read(_sockfd, buf, sizeof(buf) - 1);
            buf[size] = '\0';
            if (size > 0)
            {
                cout << "server:" << buf <<endl;
            }
            else if (size == 0)
            {
                cout << "server close" << endl;
                break;
            }
        }
        else
        {
            cout << "server close" << endl;
            break;
        }
        close(_sockfd);
        sleep(1);
    }
}