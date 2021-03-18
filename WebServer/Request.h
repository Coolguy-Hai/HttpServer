#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED
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
#include <string>
#include <assert.h>
#include <sys/epoll.h>
#define BUFFER_SIZE 4096

class Request{
public:
    Request(){}
    Request(int socket_, int epollfd_);
    ~Request(){}
public:
    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };
public:
    void parse();
    char* getUri();
    LINE_STATUS parse_line(char* buffer, int& checked_index, int& read_index);
    HTTP_CODE parse_requestline(char* temp, CHECK_STATE& checkstate);
    HTTP_CODE parse_headers(char* temp);
    HTTP_CODE parse_content(char* buffer, int& checked_index, CHECK_STATE& checkstate, int& read_index, int& start_line);
private:
    int socket;
    int epollfd;
    char uri[80];
};

#endif // REQUEST_H_INCLUDED
