#include"Client.h"
#include<csignal>

using namespace std;

Client client("172.16.0.17", 54321);


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
    print("客户端创建socket成功!!!");
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

void Client::login(string s){
    const char *msg=s.c_str();
    send(this->sockfd,msg,strlen(msg),0);
}



void sig_close(int){
    client.Close();
    exit(SUCCESS);
}
int main(){
    //CTRL+C退出
    signal(SIGINT,sig_close);

    //登录报文 seq=@^@@&* login name age intro male
    string msg="login";
    string t;
    int age;
    cout <<"输入的姓名:";
    cin>>t;
    msg = msg +"@^@@&*"+t;
    cout <<"输入年龄(0-99):";
    cin>>age;
    msg = msg +"@^@@&*"+to_string(age);
    cout <<"介绍你自己:";
    cin>>t;
    msg =msg +"@^@@&*"+t;
    cout <<"你是不是男生?(是/否)";
    cin>>t;
    msg =msg +"@^@@&*"+t;
    cout <<msg <<endl;
    client.login(msg);
    
    return 0;
}