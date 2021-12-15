#include "Server.h"
#include <csignal>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <unordered_set>
#include <cmath>
#include <ctime>

using namespace std;

int globaltime = 0;
int Server::sockfd = 1;
int idx = 0;
int lock = 0;                // 0没锁,1锁上了
vector<bool> is_match;       // id
vector<user> all;            //存储完整的信息
unordered_set<int> man;      // nid
unordered_set<int> woman;    // nid
unordered_map<int, int> map; //<id,id>

void match()
{
    cout << "匹配中!!!" << endl;

    unordered_set<int> res01;
    unordered_set<int> res02;
    for (auto i : man)
    {
        int age1 = all[i].age;
        bool flag = 0;
        for (auto j : woman)
        {
            if (is_match[j] == true)
            {
                continue;
            }
            int age2 = all[j].age;
            bool flag = false;
            if (abs(age1 - age2) <= 5 || ((globaltime - all[i].jointime) >= 60 || (globaltime - all[j].jointime) >= 60))
            {
                res01.insert(i);
                res02.insert(j);
                is_match[i] = true;
                is_match[j] = true;
                map[i] = j;
                map[j] = i;
                flag = true;
            }
        }
    }
    for (auto t : res01)
        man.erase(t);
    for (auto t : res02)
        woman.erase(t);
}

void match_server()
{
    while (true)
    {
        sleep(5);
        while (!lock & 1) //等待锁的释放
        {
            lock ^= 1;
            match();
            lock ^= 1;
            cout << "匹配完成" << endl;
            break;
        }
    }
}

void showtime()
{
    time_t nowtime=time(NULL);
    tm *ptm = localtime(&nowtime);
    cout << ptm->tm_year + 1900 << "年" << ptm->tm_mon + 1 << "月" << ptm->tm_mday << "日 ";
    cout << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " ";
}

string gettime()
{
    time_t nowtime=time(NULL);
    tm *ptm = localtime(&nowtime);
    string s = to_string(ptm->tm_year + 1900) + "年" + to_string(ptm->tm_mon + 1) + "月" + to_string(ptm->tm_mday) + "日 " +
               to_string(ptm->tm_hour) + ":" + to_string(ptm->tm_min) + ":" + to_string(ptm->tm_sec) + " ";
    return s;
}

Server::Server() {}

Server::~Server() {}

Server::Server(string IP, int port)
{
    this->server_info.sin_family = AF_INET;
    this->server_info.sin_addr.s_addr = inet_addr(IP.c_str());
    this->server_info.sin_port = htons(port);
    showtime();
    Server::print("服务器初始化!");
    bool ans = this->Create();
    if (!ans)
    {
        showtime();
        Server::print("服务器创建socket失败!!!");
        exit(CREATE_ERROR);
    }
    showtime();
    Server::print("服务器socket创建成功!!!");
    ans = this->Bind();
    if (!ans)
    {
        showtime();
        Server::print("服务器绑定socket失败!!!");
        exit(BIND_ERROR);
    }
    showtime();
    Server::print("服务器绑定socket成功!!!");
    ans = this->Listen();
    if (!ans)
    {
        showtime();
        Server::print("服务器监听socket失败!!!");
        exit(LISTEN_ERROR);
    }
    showtime();
    Server::print("服务器监听socket成功!!");
}

void Server::Close()
{
    close(Server::sockfd);
    sockfd = -1;
    showtime();
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
    showtime();
    Server::print("消息服务器开始服务!!!");
    while (true)
    {
        sockaddr_in in;
        socklen_t len = sizeof(in);
        int fd = accept(Server::sockfd, (sockaddr *)&in, &len);
        if (fd == -1)
        {
            showtime();
            Server::print("客户端连接出错!!!");
            continue;
        }
        cout << "fd=" << fd << endl;
        //启动线程服务这个客户端
        thread s(Server::server_thread, fd);
        //并行运行
        s.detach();
    }
}

void Server::server_thread(int fd)
{
    int id;
    //登录
    while (true)
    {
        char msg[1000] = {0};
        if (recv(fd, msg, sizeof(msg), 0) > 0)
        {
            vector<string> res;
            char *t = strtok(msg, "@^@@&*");
            while (t != NULL)
            {
                res.push_back(t);
                t = strtok(NULL, "@^@@&*");
            }

            if (res[0] == "login" && res[2].find_first_not_of("0123456789") == string::npos && (res[4] == "是" || res[4] == "否"))
            {
                //用户登录添加进入
                user t;
                t.jointime = globaltime;
                t.id = idx++;
                t.fd = fd;
                t.name = res[1];
                t.contro = res[3];
                t.age = atoi(res[2].c_str());
                while (!lock & 1)
                {
                    lock ^= 1;
                    if (res[4] == "是")
                    {
                        man.insert(t.id);
                    }
                    else
                    {
                        woman.insert(t.id);
                    }
                    all.push_back(t);
                    is_match.push_back(false);
                    lock ^= 1;
                    break;
                }
                showtime();
                cout << res[1] << "已添加!" << endl;
                sleep(1);
                string s =gettime()+"LOGIN SUCCESS";
                Send_message(fd, s);
                id = t.id;
                break;
            }
        }
    }
    //等待匹配
    while (true)
    {
        showtime();
        cout << fd << "等待匹配" << endl;
        sleep(1);
        if (is_match[id] == true)
        {
            showtime();
            string s = gettime() + "MATCH SUCCESS";
            cout << fd << "匹配成功" << endl;
            Send_message(fd, s);
            sleep(1);
            auto match_people = all[map[id]];
            s = gettime() + "\n" + "匹配到了:" + match_people.name + "\n" + "年龄:" + to_string(match_people.age) + "\n" + "自我介绍:" + match_people.contro + "\n";
            Send_message(fd, s);
            break;
        }
    }

    //发送消息
    while (true)
    {
        char msg[1000] = {0};
        if (recv(fd, msg, sizeof(msg), 0) == 0)
        {
            //表示有客户端退出了,从维护的map中移除
            map.erase(map[id]);
            map.erase(id);
            return;
        }
        string t = gettime() + msg;
        Send_message(all[map[id]].fd, t);
    }
}

void Server::print(string s)
{
    cout << s << endl;
}

void Server::Send_message(int fd, string msg)
{
    const char *t = msg.c_str();
    send(fd, t, strlen(t), 0);
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
    thread mt(match_server);
    thread msg_server(server.Run);
    thread timer(addtime);
    msg_server.join();
    timer.join();
    mt.join();
    return 0;
}
