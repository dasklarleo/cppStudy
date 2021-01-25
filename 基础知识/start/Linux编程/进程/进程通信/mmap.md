---

typora-root-url: ../img
---



# 存储映射区

存储映射I/O (Memory-mapped I/O) 使一个磁盘文件与存储空间中的一个缓冲区相映射。从缓冲区中取数据，就相当于读文件中的相应字节；将数据写入缓冲区，则会将数据写入文件。这样，就可在不使用read和write函数的情况下，使用地址（指针）完成I/O操作。

使用存储映射这种方法，首先应通知内核，将一个指定文件映射到存储区域中。这个映射工作可以通过mmap函数来实现。





![image-20210125114709154](/../进程通信/image-20210125114709154.png)

## MMAP

- 函数作用:
  - 建立存储映射区
- 函数原型
  - void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
-  函数返回值：
  - 成功：返回创建的映射区首地址；
  - 失败：MAP_FAILED宏
-  参数：  
  - addr:   指定映射的起始地址, 通常设为NULL, 由系统指定
  - length：映射到内存的文件长度
  -  prot：  映射区的保护方式, 最常用的:
    -  读：PROT_READ
    - 写：PROT_WRITE
    - 读写：PROT_READ | PROT_WRITE
-  flags：  映射区的特性, 可以是
  - MAP_SHARED: 写入映射区的数据会写回文件, 且允许其他映射该文件的进程共享
  - MAP_PRIVATE: 对映射区的写入操作会产生一个映射区的复制(copy-on-write), 对此区域所做的修改不会写回原文件
-  fd：由open返回的文件描述符, 代表要映射的文件
- offset：以文件开始处的偏移量, **必须是****4k的整数倍**, 通常为0, 表示从文件头开始映射。