#include "staticResourceServer.h"
void staticResourceServer::init(int epollfd, int sockfd, const sockaddr_in& client_addr){
        m_epollfd = epollfd;
        m_sockfd = sockfd;
        m_address = client_addr;
        memset(m_buf, '\0', BUFFERSIZE);
        m_read_idx = 0;
}
void staticResourceServer::process(){
    Request request = Request(m_sockfd);
    request.parse();
    Response response = Response(m_sockfd);
    response.setRequest(request);
    response.sendMessage();
}

