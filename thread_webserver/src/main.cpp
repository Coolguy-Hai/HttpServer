#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdarg.h>

#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"

#include "Thread.h"
#include "EventLoop.h"

#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000

extern int addfd(int epollfd, int fd);
extern int removefd(int epollfd, int fd);

using namespace std;

void addsig(int sig, void(handler)(int), bool restart = true){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if(restart){
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}
void show_error(int connfd, const char* info){
    printf("%s", info);
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <stdio.h>

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): new connection [%s] from %s\n",
           conn->name().c_str(),
           conn->peerAddress().toHostPort().c_str());
  }
  else
  {
    printf("onConnection(): connection [%s] is down\n",
           conn->name().c_str());
  }
}

void onMessage(const TcpConnectionPtr& conn,
               const char* data,
               ssize_t len)
{
  printf("onMessage(): received %zd bytes from connection [%s]\n",
         len, conn->name().c_str());
}

int main(int argc, char* argv[])
{
  printf("main(): pid = %d\n", getpid());

  InetAddress listenAddr(9981);
  EventLoop loop;

  TcpServer server(&loop, listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  if (argc > 1) {
    server.setThreadNum(atoi(argv[1]));
  }
  server.start();

  loop.loop();
}
//int main(int argc, char* argv[])
//{
//    if(argc <= 2){
//        printf("usage: %s ip_address port_number\n", argv[0]);
//        return 1;
//    }
//    const char* ip = argv[1];
//    int port = atoi(argv[2]);
//
//    addsig(SIGPIPE, SIG_IGN);
//    threadpool<http_conn>* pool = NULL;
//    try{
//        pool = new threadpool<http_conn>;
//    }
//    catch(...){
//        return 1;
//    }
//    http_conn* users = new http_conn[MAX_FD];
//    assert(users);
//    int user_count = 0;
//
//    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
//    assert(listenfd >= 0);
//    struct linger tmp = {1, 0};
//    setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
//    int ret = 0;
//    struct sockaddr_in address;
//    bzero(&address, sizeof(address));
//    address.sin_family = AF_INET;
//    inet_pton(AF_INET, ip, &address.sin_addr);
//    address.sin_port = htons(port);
//    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
//    assert(ret >= 0);
//
//    ret = listen(listenfd, 5);
//    assert(ret >= 0);
//
//    epoll_event events[MAX_EVENT_NUMBER];
//    int epollfd = epoll_create(5);
//    assert(epollfd != -1);
//    addfd(epollfd, listenfd);
//    http_conn::m_epollfd = epollfd;
//
//    while(true){
//        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
//        if((number < 0) && (errno != EINTR)){
//            printf("epoll failure\n");
//            break;
//        }
//        for(int i = 0;i < number;i++){
//            int sockfd = events[i].data.fd;
//            if(sockfd == listenfd){
//                struct sockaddr_in client_address;
//                socklen_t client_addrlength = sizeof(client_address);
//                int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
//                if(connfd < 0){
//                    printf("errno is:%d\n", errno);
//                    continue;
//                }
//                if(http_conn::m_user_count >= MAX_FD){
//                    show_error(connfd, "INTERNAL server busy");
//                    continue;
//                }
//                users[connfd].init(connfd, client_address);
//                printf("connfd:%d\n", connfd);
//            }
//            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
//                users[sockfd].close_conn();
//            }
//            else if(events[i].events & EPOLLIN){
//                if(users[sockfd].read()){
//                    pool->append(users+sockfd);
//                }
//                else{
//                    users[sockfd].close_conn();
//                }
//            }
//            else if(events[i].events & EPOLLOUT){
//                if(!users[sockfd].write()){
//                    users[sockfd].close_conn();
//                }
//            }
//            else {}
//        }
//    }
//    close(epollfd);
//    close(listenfd);
//    delete [] users;
//    delete pool;
//    return 0;
//}
