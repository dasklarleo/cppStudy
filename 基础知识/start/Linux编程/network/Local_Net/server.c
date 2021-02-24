//
// Created by 刘玉晓 on 2/17/21.
//

#include <unp.h>
#include <ctype.h>

int main(){
    int lfd=Socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un serv;
    serv.sun_family=AF_UNIX;
    strcpy(serv.sun_path,"./serv.sock");
    Bind(lfd,(SA*)&serv, sizeof(serv));

    Listen(lfd,128);

    struct sockaddr_un client;
    int len= sizeof(client);
    int cfd=Accept(lfd,(SA*)&client,&len);

    char buf[MAXLINE];
    while (1){
        memset(buf,0x00, sizeof(buf));
        int n=Read(cfd,buf, sizeof(buf));
        for (int i = 0; i < n; ++i) {
            buf[i]=toupper(buf[i]);
        }
        Write(cfd,buf, sizeof(buf));
    }
}