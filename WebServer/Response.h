#ifndef RESPONSE_H_INCLUDED
#define RESPONSE_H_INCLUDED
#include "Request.h"
#include <sys/stat.h>
#include <sys/sendfile.h>

class Response{
public:
    Response(){}
    Response(int socket_, int epollfd_);
    ~Response(){}
public:
    void setRequest(Request request_);
    void sendMessage();
private:
    Request request;
    int socket;
    int epollfd;
};

#endif // RESPONSE_H_INCLUDED
