#include "Server.h"
#include <csignal>

using namespace std;

Server server("172.16.0.17", 54321);
int globaltime =0;

void run_server()
{
    server.Run();
}

void sig_close(int)
{
    server.Close();
    exit(SUCCESS);
}

void addtime(){
    while (true)
    {
        sleep(1);
        globaltime++;
        cout<< globaltime<<endl;
    }
}

int main()
{
    // CTRL+C退出
    signal(SIGINT, sig_close);
    thread msg_server(run_server);
    thread timer(addtime);
    msg_server.join();  
    timer.join();
    return 0;
}