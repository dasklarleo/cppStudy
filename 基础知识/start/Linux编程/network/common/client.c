#include <unp.h>
int main()
{ //声明套接字,获得文件描述符 在内核有发送和接受缓冲区
    int socket = Socket(AF_INET, SOCK_STREAM, 0);
    //Connect()
    //连接
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port=htons(10003);
    inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
    Connect(socket, (SA *)&serv, sizeof(serv));

    //收发数据
    char sendbuf[4096];
    int n=0;
    while(1){
        memset(sendbuf,0x00,sizeof(sendbuf));
        n=Read(STDIN_FILENO,sendbuf,sizeof(sendbuf));

        //发送数据
        Write(socket,sendbuf,n);
        //读数据
    }
    close(socket);
}