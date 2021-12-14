#ifndef __CLIENT__H
#define __CLIENT__H

#include<cstdlib>
#include<string>
#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>

#define SUCCESS 1
#define CREATE_ERROR -1
#define CONNET_ERROR -2

using namespace std;
class Client
{
private:
    int sockfd;
    sockaddr_in server_adress;
    bool Create();
    bool Connet();
    
public:
    Client(string adress,int port);
    ~Client();
    void print(string s);
    void Close();
    void login(string s);
};


#endif