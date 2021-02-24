//
// Created by 刘玉晓 on 2/17/21.
//


#include <unp.h>
#include <ctype.h>
#include <event2/event.h>
//非未决态：没有上树
//未决态：上树了
//
//1 创建socket
//2 设置端口复用
//3 绑定bind
//4 设置监听
//5 创建地基
//6 创建lfd对应的事件
struct event *connev = NULL;

void readcb(evutil_socket_t cfd, short events, void *args) {
    int n;
    char buf[MAXLINE];

    n = read(cfd, buf, sizeof(buf));
    if (n <= 0) {
        close(cfd);
        event_del(connev);
        return;
    }
    for (int i = 0; i < n; ++i) {
        buf[i] = toupper(buf[i]);
    }
    Write(cfd, buf, n);

}

void conncb(evutil_socket_t lfd, short events, void *args) {
    //获取base
    struct event_base *base = (struct event_base *) args;

    //创建新的通信文件描述符
    int cfd = Accept(lfd, NULL, NULL);

    //创建通信文件描述符的事件
    struct event *connev = event_new(base, cfd, EV_READ | EV_PERSIST, readcb, NULL);
//    if (connev == NULL) {
//        close(cfd);
//        event_base_loopexit(base,NULL);
//    }
    //将通信文件描述符对应的事件上event_base地基
    event_add(connev, NULL);
}

int main() {
    int opt = 1;
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);
    Setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //绑定
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(10000);
    Bind(lfd, (SA *) &server, sizeof(server));

    //监听
    Listen(lfd, 128);

    //创建地基
    struct event_base *base = event_base_new();
    if (base == NULL) {
        printf("event_base_new error");
        exit(-1);
    }

    //创建事件
    struct event *ev = event_new(base, lfd, EV_READ | EV_PERSIST, conncb, base);//最后的是传入的参数
    if (ev == NULL) {
        printf("event_new error");
        exit(-1);
    }

    //将新的事件节点上到地基上面
    event_add(ev, NULL);

    //进入循环等待
    event_base_dispatch(base);

    //释放资源
    event_base_free(base);
    event_free(ev);

    close(lfd);

    exit(0);
}