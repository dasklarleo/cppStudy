#include <unp.h>
#include <ctype.h>
#include <pthread.h>
void *threadWork(void *args)//不可以关闭父线程lfd,因为线程共享一个变量
{   
    int cfd = *(int *)args;
    char buf[MAXLINE];
    int n;
    while (1)
    {
        n = Read(cfd, buf, sizeof(buf));
        if (n <= 0)
        {
            printf("connection failed with client");
            break;
        }
        printf("recv data:%s", buf);
    }
    close(cfd);
    //不用使用exit
    pthread_exit(NULL);
}
int main()
{
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(10003);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(lfd, (SA *)&serv, sizeof(serv));

    Listen(lfd, 128);

    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    pthread_t threadID;
    while (1)
    {   //取出连接
        int cfd = Accept(lfd, NULL, NULL);

        //创建子线程
        pthread_create(&threadID, NULL, threadWork, &cfd);

        //分离属性
        pthread_detach(threadID);
    }
    close(lfd);
    return 0;
}