#include "Response.h"
#include "WebServer.h"
#include "processpool.h"

const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";

void Response::setRequest(Request request_){
    request = request_;
}
void Response::sendMessage(){
    char dir[80];
    getcwd(dir, sizeof(dir));
    char* uri = request.getUri();
    strcat(dir, uri);
    int filefd = open(dir, O_RDONLY);
    if(filefd == -1){
        send(socket, res, strlen(res), 0);
        return;
    }
    struct stat stat_buf;
    fstat(filefd, &stat_buf);
    int cont_len = stat_buf.st_size;
    char status[100];
    sprintf(status, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n", cont_len);
    send(socket, status, strlen(status), 0);
    printf("%s\n", request.getUri());
    sendfile(socket, filefd, NULL, stat_buf.st_size);
    removefd(epollfd, socket);
}
Response::Response(int socket_, int epollfd_){
    socket = socket_;
    epollfd = epollfd_;
}
