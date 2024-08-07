#pragma once
#include <map>
#include <string>
#include <iostream>
using namespace std;
class dictionaries
{
public:
    dictionaries() {}
    void Init(){
        dictionary.emplace(make_pair("hello","你好"));
    }
    string Find(const string english)
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