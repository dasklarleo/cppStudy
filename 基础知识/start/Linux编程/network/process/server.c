#include <unp.h>
#include <ctype.h>
int main()
{
    int socket = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(10003);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(socket, (SA *)&serv, sizeof(serv));

    Listen(socket, 128);

    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    while (1)
    {
        int client = Accept(socket, (SA *)&clientAddr, &len);

        pid_t pid = Fork();
        if (pid > 0)
        {
            close(client);
        }
        if (pid == 0)
        {
            char buf1[4096];
            char tempbuf[16];
            while (1)
            {
                close(socket);

                memset(buf1, 0x00, sizeof(buf1));
                int n = Read(client, buf1, sizeof(buf1));
                if (n == 0)
                {
                    break;
                }
                printf("1");
                printf("server has received data[%s] from client [%s] with port==[%d]\n", 
                       buf1,
                       inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, tempbuf, sizeof(tempbuf)),
                       ntohs(clientAddr.sin_port));
            }

            printf("connetcion failed with[%s]", inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, tempbuf, sizeof(tempbuf)));
            close(client);
            exit(0);
        }
    }
}