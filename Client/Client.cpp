#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <string>
using namespace std;

bool match = false;
int sockfd;
char *IP = "42.193.188.120";
short PORT = 54321;
typedef struct sockaddr SA;
//启动客户端，连接服务器
void init()
{
    printf("聊天室客户端开始启动\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if (connect(sockfd, (SA *)&addr, sizeof(addr)) == -1)
    {
        perror("无法连接到服务器");
        printf("客户端启动失败\n");
        exit(-1);
    }
    printf("客户端启动成功\n");
}
//开始通信
void *recv_thread(void *p)
{ //收消息
    while (1)
    {
        char buf[1000] = {0};
        if (recv(sockfd, buf, sizeof(buf), 0) <= 0)
        {
            return NULL;
        }
        printf("%s\n", buf);
        string t = buf;
        if (strstr(t.c_str(),"MATCH SUCCESS")!=NULL&&!match)
        {
            cout << "匹配成功" << endl;
            match = true;
        }
        if (!match)
        {
            cout<<"匹配中!!!请等待!!!"<<endl;
        }
        
        
    }
}

void start()
{
    //发送消息
    //发消息之前，启动一个线程,用来接受服务器发送过来的消息
    pthread_t pid;
    pthread_create(&pid, 0, recv_thread, 0);
    while (1)
    {
        char buf[100] = {};
        scanf(" %s", buf); //接受用户输入
        if (!match)
        {
            cout << "还没有匹配成功！" << endl;
            continue;
        }

        send(sockfd, buf, strlen(buf), 0); //发给服务器
    }
}

void sig_close(int)
{
    //关闭客户端的描述符
    close(sockfd);
    exit(0);
}
int main()
{
    string seq = "@^@@&*";
    signal(SIGINT, sig_close); //关闭CTRL+C
    string s = "login";
    string t;
    cout << "请输入您的昵称:";
    cin >> t;
    s = s + seq + t;
    cout << "请输入您的年龄:";
    cin >> t;
    s = s + seq + t;
    cout << "请输入您的简介:";
    cin >> t;
    s = s + seq + t;
    cout << "您是男生吗?(是/否)";
    cin >> t;
    s = s + seq + t;
    const char *msg = s.c_str();
    init();                            //连接服务器
    send(sockfd, msg, strlen(msg), 0); //将昵称发给服务器
    start();                           //开始通信
    return 0;
}