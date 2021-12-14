#ifndef __CLIENT__H
#define __CLIENT__H

#include<cstdlib>
#include<string>
#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>

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
};

Client::Client(string IP,int port)
{
    print("客户端开始启动!!!");
    this->server_adress.sin_family=AF_INET;
    this->server_adress.sin_addr.s_addr=inet_addr(IP.c_str());
    this->server_adress.sin_port=htons(port);

    bool ans =Create();
    if (!ans)
    {
       print("客户端socket失败!!!");
       exit(CREATE_ERROR);
    }
    print("客户端创建socket失败!!!");
    ans = Connet();
    if (!ans)
    {
        print("客户端连接服务器失败!!!");
        exit(CONNET_ERROR);
    }
    print("客户端连接服务器成功!!!");
    
}
Client::~Client(){}

bool Client::Create(){
    this->sockfd = socket(AF_INET,SOCK_STREAM,0);
    return this->sockfd!=-1;
}

bool Client::Connet(){
    return !(connect(this->sockfd,(sockaddr*)&this->server_adress,sizeof(this->server_adress)) ==-1);
}

void Client::Close(){
    close(this->sockfd);
}
void Client::print(string s){
    cout <<s <<endl;
}


#endif