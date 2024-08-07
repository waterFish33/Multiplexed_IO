#include "epoll.hpp"
#include <iostream>
using namespace std;
int main(int argc,char** argv){
    if(argc!=2){
        cout<<"please enter ./epoll -port"<<endl;
        return 1;
    }
    epoll_server es(stoi(argv[1]));
    es.Init();
    es.Listen();
    es.Start();
    return 0;

}