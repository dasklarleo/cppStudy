#include <unp.h>
//select同时监听多个文件描述符,将监听的操作交给内核进行,内核会告诉我们消息
//nfds:输入参数，告诉要监听文件描述符的范围，一般为最大文件描述符+1
//readfds：输入参数（要监听的描述符），也是输出参数（文件描述符的变化）
//excepfds：输入输出参数 表示异常的事件
//socket开发流程
    //创建socket
    //设置端口复用
    //绑定
    //设置监听
    //监听这个文件描述符：
        //fd_set readfds
        //socket加入readfds
    //while（1）seletct
int main()
{

}