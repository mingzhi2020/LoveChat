#ifndef __SERVER__H
#define __SERVER__H

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include<vector>

#define SUCCESS 1
#define CREATE_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3
#define RUNTIME_ERROR -4

using namespace std;

struct user
{
    int id;
    int fd;
    string name;
    bool ismale;
    double x;
    double y;
    int jointime;
};


class Server
{
private:
    sockaddr_in server_info;
    
    bool Create();
    bool Bind();
    bool Listen();

public:
    static int sockfd;
    Server();
    ~Server();
    Server(string address, int port);
    static void Run();
    void Close();
    static void print(string s);
    static void server_thread(int fd);
};

#endif