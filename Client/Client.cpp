#include"Client.h"
#include<csignal>

using namespace std;

Client client("172.16.0.17", 54321);

void sig_close(int){
    exit(SUCCESS);
}
int main(){
    signal(SIGINT,sig_close);
    return 0;
}