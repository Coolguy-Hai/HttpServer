#ifndef STATICRESOURCESERVER_H_INCLUDED
#define STATICRESOURCESERVER_H_INCLUDED
#include "Request.h"
#include "Response.h"

class staticResourceServer{
public:
    staticResourceServer(){}
    ~staticResourceServer(){}
    void init(int epollfd, int sockfd, const sockaddr_in& client_addr);
    void process();
private:
    static const int BUFFERSIZE = 1024;
    static int m_epollfd;
    int m_sockfd;
    sockaddr_in m_address;
    char m_buf[BUFFERSIZE];
    int m_read_idx;
};
void staticResourceServer::init(int epollfd, int sockfd, const sockaddr_in& client_addr){
        m_epollfd = epollfd;
        m_sockfd = sockfd;
        m_address = client_addr;
        memset(m_buf, '\0', BUFFERSIZE);
        m_read_idx = 0;
}
void staticResourceServer::process(){
    Request request = Request(m_sockfd, m_epollfd);
    request.parse();
    Response response = Response(m_sockfd, m_epollfd);
    response.setRequest(request);
    response.sendMessage();
}
int staticResourceServer::m_epollfd = -1;
#endif // STATICRESOURCESERVER_H_INCLUDED
