//
// Created by 刘玉晓 on 2/17/21.
//

#include <unp.h>

int main() {
    int cfd = Socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(10000);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

    int nReady;
    int len = sizeof(server);
    char buf[MAXLINE];
    while (1) {
        memset(buf, 0x00, sizeof(buf));
        nReady=Read(STDIN_FILENO, buf, sizeof(buf));
        Sendto(cfd, buf, nReady, 0, (SA *) &server, len);
        Recvfrom(cfd, buf, sizeof(buf), 0, (SA *) &server, &len);
        Write(STDOUT_FILENO, buf, strlen(buf));
    }
}