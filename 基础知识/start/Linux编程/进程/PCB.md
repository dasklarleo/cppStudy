程控制块是task_struct结构体

/usr/src/linux-headers-4.4.0-96/include/linux/sched.h文件的1390行处可以查看struct task_struct 结构体定义。其内部成员有很多，我们重点掌握以下部分即可：

-  进程id。系统中每个进程有唯一的id，在C语言中用pid_t类型表示，其实就是一个非负整数
- 进程的状态，有就绪、运行、挂起、停止等状态
- 进程切换时需要保存和恢复的一些CPU寄存器
- 描述虚拟地址空间的信息
- 描述控制终端的信息
-  当前工作目录（Current Working Directory）
  - getcwd --pwd
- umask掩码。
-  文件描述符表，包含很多指向file结构体的指针。
-  和信号相关的信息
- 用户id和组id
- 会话（Session）和进程组
- 进程可以使用的资源上限（Resource Limit）
  - n ulimit -a