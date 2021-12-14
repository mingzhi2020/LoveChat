#ifndef __SERVER__H
#define __SERVER__H

#include<arpa/inet.h>
#include<iostream>
#include<sys/socket.h>
#include<thread>
#include<unistd.h>
#include<string>
#include<cstdlib>
#include<unordered_map>

#define SUCCESS 1
#define CREATE_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3
#define RUNTIME_ERROR -4

using namespace std;

class Server
{
private:
    sockaddr_in server_info;
    int sockfd;
    bool Create();
    bool Bind();
    bool Listen();
public:
    Server();
    ~Server();
    Server(string address,int port);
    void Run();
    void Close();
    void print(string s);
};

Server::Server(){}

Server::~Server(){}

Server::Server(string address,int port){
    this->server_info.sin_family =AF_INET;
    this->server_info.sin_addr.s_addr = inet_addr(address.c_str());
    this->server_info.sin_port = htons(port);
    print("服务器初始化!");
    bool ans =this->Create();
    if(!ans){
        print("服务器创建socket失败!!!");
        exit(CREATE_ERROR);
    }
    print("服务器socket创建成功!!!");
    ans =this->Bind();
    if(!ans){
        print("服务器绑定socket失败!!!");
        exit(BIND_ERROR);
    }
    print("服务器绑定socket成功!!!");
    ans =this->Listen();
    if(!ans){
        print("服务器监听socket失败!!!");
        exit(LISTEN_ERROR);
    }
    print("服务器监听socket成功!!");
}

void Server::Close(){
    close(this->sockfd);
    print("服务器已关闭!!!");
}

bool Server::Create(){
    this->sockfd = socket(AF_INET,SOCK_STREAM,0);
    return this->sockfd != -1;
}

bool  Server::Bind(){
    return !(bind(this->sockfd,(sockaddr*)&this->server_info,sizeof(this->server_info))==-1);
}

bool Server::Listen(){
    return !(listen(this->sockfd,2000)==-1);
}

void Server::Run(){
    print("消息服务器开始服务!!!");
    while (true)
    {
        sockaddr_in in;
        socklen_t len = sizeof(in);
        int fd = accept(this->sockfd,(sockaddr*)&in,&len);
        if (fd==-1)
        {
            print("客户端连接出错!!!");
            continue;
        }
        cout << "fd="<<fd<<endl;
    }
    
}

void Server::print(string s){
    cout << s<<endl;
}

#endif