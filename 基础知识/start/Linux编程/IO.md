# 示例代码

```c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	//打开文件
	int oldfd = open(argv[1], O_RDWR | O_CREAT, 0755);
	if(oldfd<0)
	{
		perror("open error");
		return -1;
	}

	int newfd = open(argv[2], O_RDWR | O_CREAT, 0755);
	if(newfd<0)
	{
		perror("open error");
		return -1;
	}
	//调用dup2函数复制fd
	dup2(oldfd, newfd);
	printf("newfd:[%d], oldfd:[%d]\n", newfd, oldfd);

	//使用fd对文件进行写操作
	write(newfd, "hello world", strlen("hello world"));

	//调用lseek函数移动文件指针到开始处
	lseek(newfd, 0, SEEK_SET);

	//使用newfd读文件
	char buf[64];
	memset(buf, 0x00, sizeof(buf));
	int n = read(oldfd, buf, sizeof(buf));
	printf("read over: n==[%d], buf==[%s]\n", n, buf);

	//关闭文件
	close(oldfd);
	close(newfd);

	return 0;
}

```

## open函数

1. 打开方式
   1. 只能写一个的读写方式
      1. O_RDONLY：只读
      2. O_WRONLY：只写
      3. O_RDWR：可读可写
   2. 补充：
      1. O_APPEND:表示追加内容，之前的内容不进行覆盖
      2. O_CREAT:文件如果不存在就进行追加
      3. O_EXCL:文件如果已经存在，且同时指定了O_CREAT，返回错误
      4. O_TRUNC:文件存在，长度截断为0
      5. O_NONBLOCK:对于设备文件, 以O_NONBLOCK方式打开可以做非阻塞I/O(NonblockI/O),非阻塞I/O
2. 返回值：
   1. 成功：返回一个最小的没有被占用的文件描述符
   2. 错误：返回-1，并设置errno

## close函数

参数为文件描述符

返回值：

1. 成功为0
2. 错误为-1

## read函数

1. 功能：从打开的设备或者文件描述符之中读取数据
2. 函数原型：`ssize_t read(int fd, void *buf, size_t count)`
3. 参数
   1. fd：文件描述符
   2. buf：读取到哪一个缓冲区
   3. count：读几个
4. 返回值：
   1. \>0：读取到的字节数
   2. \> 0：文件读取完毕
   3. =-1： 出错，并设置errno

## write函数

1. 功能：向打开的设备或文件中写数据
2. 函数原型：`ssize_t write(int fd, const void *buf, size_t count)`
3. 函数参数：
   1. fd：文件描述符
   2. buf：写入的数据所在的缓冲区
   3. count：写多少个字节
4. 返回：
   1. 成功：返回写入的字节
   2. 错误：返回-1并设置errno

## lseek函数

所有打开的文件都有一个当前**文件偏移量(current file offset)**,以下简称为cfo. cfo通常是一个非负整数, 用于表明文件开始处到文件当前位置的字节数. 读写操作通常开始于 cfo, 并且使 cfo 增大, 增量为读写的字节数. 文件被打开时, **cfo 会被初始化为 0, 除非使用了 O_APPEND.**

1. 功能：改变cfo
   1. 函数原型：`off_t lseek(int fd, off_t offset, int whence)`
   2. 参数：
      1. fd：文件描述符
      2. offset：偏移
      3. whence：
         1. SEEK_SET:偏移量为offset
         2. SEEK_CUR:偏移量为cfo+offset
         3. SEEK_END:：文件偏移量为offset+文件长度

