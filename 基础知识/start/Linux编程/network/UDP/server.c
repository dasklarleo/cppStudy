//
// Created by 刘玉晓 on 2/17/21.
//

#include <unp.h>
#include <ctype.h>

//1 创建socket
//2 绑定bind
//3 while（1）
//读取数据
//发送数据
//close

int main() {
    int cfd = Socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server, client;
    bzero(&server, sizeof(server));
    bzero(&client, sizeof(client));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(10000);
    server.sin_family = AF_INET;
    Bind(cfd, (SA *) &server, sizeof(server));

    int nRead;
    char buf[MAXLINE];
    int len= sizeof(client);
    while (1) {
        memset(buf, 0x00, sizeof(buf));
        nRead = Recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr *) &client, &len);
        for (int i = 0; i < nRead; ++i) {
            buf[i] = toupper(buf[i]);
        }
        printf("recv data from port==[%d]",ntohs(client.sin_port));
        Sendto(cfd,buf,nRead,0,(SA*)&client, len);
    }
    Close(cfd);
    return 0;

}