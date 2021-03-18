#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED
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
#include <sys/epoll.h>
#define BUFFER_SIZE 4096

static const char* szret[] = {"I get correct result.\n", "Something wrong.\n"};
static const char* res = "HTTP/1.1 404 File Not Found\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\n<h1>File Not Found</h1>";
class WebServer{
public:
    WebServer(){}
    WebServer(const char* ip_, int port_);
    ~WebServer(){}
public:
    void await();
private:
    const char* ip;
    int port;
};

#endif // WEBSERVER_H_INCLUDED
