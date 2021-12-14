#include "Server.h"
#include <csignal>

using namespace std;

int globaltime = 0;
int Server::sockfd=1;

Server::Server() {}

Server::~Server() {}

Server::Server(string IP, int port)
{
    this->server_info.sin_family = AF_INET;
    this->server_info.sin_addr.s_addr = inet_addr(IP.c_str());
    this->server_info.sin_port = htons(port);
    Server::print("服务器初始化!");
    bool ans = this->Create();
    if (!ans)
    {
        Server::print("服务器创建socket失败!!!");
        exit(CREATE_ERROR);
    }
    Server::print("服务器socket创建成功!!!");
    ans = this->Bind();
    if (!ans)
    {
        Server::print("服务器绑定socket失败!!!");
        exit(BIND_ERROR);
    }
    Server::print("服务器绑定socket成功!!!");
    ans = this->Listen();
    if (!ans)
    {
        Server::print("服务器监听socket失败!!!");
        exit(LISTEN_ERROR);
    }
    Server::print("服务器监听socket成功!!");
}

void Server::Close()
{
    close(Server::sockfd);
    Server::print("服务器已关闭!!!");
}

bool Server::Create()
{
    Server::sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return Server::sockfd != -1;
}

bool Server::Bind()
{
    return !(bind(Server::sockfd, (sockaddr *)&this->server_info, sizeof(this->server_info)) == -1);
}

bool Server::Listen()
{
    return !(listen(Server::sockfd, 2000) == -1);
}

void Server::Run()
{
    Server::print("消息服务器开始服务!!!");
    while (true)
    {
        sockaddr_in in;
        socklen_t len = sizeof(in);
        int fd = accept(Server::sockfd, (sockaddr *)&in, &len);
        if (fd == -1)
        {
            Server::print("客户端连接出错!!!");
            continue;
        }
        cout << "fd=" << fd << endl;
        //启动线程服务这个客户端
        thread s(Server::server_thread,fd);
        s.detach();
    }
}

void Server::server_thread(int fd){
    //while (true)
    //{
        char msg[1000]={0};
        if (recv(fd,msg,sizeof(msg),0)==0)
        {
            //表示有客户端退出了
        }
        string t =msg;
        cout <<t <<endl;
    //}
}

void Server::print(string s)
{
    cout << s << endl;
}




Server server("172.16.0.17", 54321);
void sig_close(int)
{
    server.Close();
    exit(SUCCESS);
}

void addtime()
{
    while (true)
    {
        sleep(1);
        globaltime++;
        cout << globaltime << endl;
    }
}

int main()
{
    // CTRL+C退出
    signal(SIGINT, sig_close);
    thread msg_server(server.Run);
    thread timer(addtime);
    msg_server.join();
    timer.join();
    return 0;
}