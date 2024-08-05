#include <iostream> 
#include "select.hpp"

using namespace std;
int main(int argc, char **argv)
{
    if (argc<2){
        cout<<"please enter ./select -port"<<endl;
    }
        Select sl(stoi(argv[1]));
    sl.Init();
    
sl.Listen();
   sl.Server();
    
    return 0;
}
