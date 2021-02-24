//
// Created by 刘玉晓 on 2/17/21.
//
#include <unp.h>

int main() {
    int nready;//返回准备好数据的个数
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);

    //设置端口复用
    int opt = 1;
    Setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

    //绑定bind
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(10002);
    svraddr.sin_addr.s_addr = htonl("127.0.0.1");
    Bind(lfd,(SA*)&svraddr, sizeof(svraddr));

    //监听Listen
    Listen(lfd,128);

    //创建一棵epoll树

}