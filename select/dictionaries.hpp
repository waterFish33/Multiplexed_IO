#pragma once
#include <map>
#include <string>
using namespace std;
class dictionaries
{
public:
    dictionaries() {}
    void init(){
        dictionary.emplace(make_pair("hello","你好"));
    }
    string find(const string english)
    {
        cout<<"1:"<<english<<endl;
        auto it=dictionary.find(english);
        if(it==dictionary.end()){
            return "没有此单词";
        }
        
        return it->second;
    }
    ~dictionaries() {}

private:
    map<string, string> dictionary;
};