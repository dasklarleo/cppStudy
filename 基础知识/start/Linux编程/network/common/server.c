#include <unp.h>
#include <ctype.h>
//socket流程
//服务端
//创建socket，返回lid
//lfd与IP、Port进行绑定
//lfd进行被动监听
//接受一个新的连接，得到一个文件描述符 accept
//while(1)
//close(lfd) close(newfd)
int main()
{ //声明套接字
    int socket = Socket(AF_INET, SOCK_STREAM, 0);

    //sockarr结构体
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(10000);
    serv.sin_addr.s_addr = htonl(INADDR_ANY); //使用本机任意可用IP

    //绑定
    Bind(socket, (struct sockaddr *)&serv, sizeof(serv));

    //监听
    Listen(socket, 128);
    //取出一个连接
    struct sockaddr_in client;
    socklen_t len=sizeof(client);
    char tempbuf[16];
    int cfd = Accept(socket, (SA*)&client, &len);
    printf("client ip addr=[%s]\nclient port==[%d]\n",
            inet_ntop(AF_INET,&client.sin_addr.s_addr,tempbuf,sizeof(tempbuf)),
            ntohs(client.sin_port));

    //处理任务
    int n = 0;
    char buf[4096];
    while (1)
    {
        memset(buf, 0x00, sizeof(buf));
        n = Read(cfd, buf, sizeof(buf));
        printf("n==[%d]\n", n);
        for (int i = 0; i < n; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        //发送数据
        write(cfd, buf, n);
    }
    //关闭两个描述符
    close(socket);
    close(cfd);
}