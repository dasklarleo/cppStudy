cmake_minimum_required(VERSION 3.14)
project(network C)

set(CMAKE_C_STANDARD 11)

# add extra include directories
include_directories(/usr/local/Cellar/libevent/2.1.11_1/include)
# add extra lib directories
link_directories(/usr/local/Cellar/libevent/2.1.11_1/lib)

add_executable(epoll epoll/epoll.c)
add_executable( simplethread_poll  thread_poll/threadpoolsimple.h thread_poll/threadpoolsimple.c)
add_executable(complexThreadPool thread_poll/threadpool.c thread_poll/threadpool.h)
add_executable(UDPServer UDP/server.c)
add_executable(UDPClient UDP/client.c)
add_executable(LocalServer Local_Net/server.c)
add_executable(LocalClient Local_Net/cleint.c)
add_executable(LibeventServer LibEvent/server.c)


target_link_libraries(UDPClient unp)
target_link_libraries(UDPServer unp)
target_link_libraries(epoll unp)
target_link_libraries(LocalServer unp)
target_link_libraries(LocalClient unp)
target_link_libraries(LibeventServer unp)
target_link_libraries(LibeventServer event)