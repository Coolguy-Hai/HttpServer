#include "WebServer.h"
#include "Request.h"
#include "Response.h"
#include "processpool.h"
#include "staticResourceServer.h"

void WebServer::await(){
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    pid_t childpid;
    assert(listenfd >= 0);
    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(listenfd, 5);
    assert(ret != -1);
    processpool<staticResourceServer>* pool = processpool<staticResourceServer>::create(listenfd);
    if(pool){
        pool->run();
        delete pool;
    }
    close(listenfd);
/*
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    for(;;){
        int fd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
        if(fd < 0){
            printf("errno is: %d\n", errno);
        }
        if((childpid = fork()) == 0){
            close(listenfd);
            Request request = Request(fd);
            request.parse();
            Response response = Response(fd);
            response.setRequest(request);
            response.sendMessage();
            exit(0);
        }
        close(fd);
    }*/
}
WebServer::WebServer(const char* ip_, int port_){
    ip = ip_;
    port = port_;
}
