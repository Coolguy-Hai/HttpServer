#include "Request.h"
#include "processpool.h"

Request::Request(int socket_, int epollfd_){
    socket = socket_;
    epollfd = epollfd_;
}
void Request::parse(){
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);
    int data_read = 0;
    int read_index = 0;
    int checked_index = 0;
    int start_line = 0;

    CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
    while(1){
        data_read = recv(socket, buffer+read_index, BUFFER_SIZE-read_index, 0);
        if(data_read == -1){
            printf("reading failed.\n");
            //removefd(epollfd, socket);
            break;
        }
        else if(data_read == 0){
            printf("remote host closes the connection.\n");
            //removefd(epollfd, socket);
            break;
        }
        read_index += data_read;
        HTTP_CODE result = parse_content(buffer, checked_index, checkstate, read_index, start_line);

        if(result == NO_REQUEST){
            continue;
        }
        else if(result == GET_REQUEST){
            //send(socket, szret[0], strlen(szret[0]), 0);
            break;
        }
        else{
            //send(socket, szret[1], strlen(szret[1]), 0);
            break;
        }
    }
}
char* Request::getUri(){
    return uri;
}
Request::LINE_STATUS Request::parse_line(char* buffer, int& checked_index, int& read_index){
    char temp;
    for(;checked_index < read_index;checked_index++){
        temp = buffer[checked_index];
        if(temp == '\r'){
            if((checked_index+1) == read_index){
                return LINE_OPEN;
            }
            else if(buffer[checked_index+1] == '\n'){
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if(temp == '\n'){
            if((checked_index > 1) && buffer[checked_index - 1] == '\r'){
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}
Request::HTTP_CODE Request::parse_requestline(char* temp, CHECK_STATE& checkstate){
    char* url = strpbrk(temp, " \t");
    if(!url){
        return BAD_REQUEST;
    }
    *url++ = '\0';

    char* method = temp;
    if(strcasecmp(method, "GET") == 0){
        printf("The request method is GET\n");
    }
    else{
        return BAD_REQUEST;
    }

    url += strspn(url, " \t");
    char* version = strpbrk(url, " \t");
    if(!version){
        return BAD_REQUEST;
    }
    *version++ = '\0';
    version += strspn(version, " \t");
    if(strcasecmp(version, "HTTP/1.1") != 0){
        return BAD_REQUEST;
    }
    if(strncasecmp(url, "http://", 7) == 0){
        url += 7;
        url = strchr(url, '/');
    }
    if(!url || url[0] != '/'){
        return BAD_REQUEST;
    }
    strcpy(uri, url);

    checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;

}
Request::HTTP_CODE Request::parse_headers(char* temp){
    if(temp[0] == '\0'){
        return GET_REQUEST;
    }
    else if(strncasecmp(temp, "Host:", 5) == 0){
        temp += 5;
        temp += strspn(temp, " \t");
        printf("the request host is:%s\n", temp);
    }
    else{
        printf("I can not handle this header\n");
    }
    return NO_REQUEST;
}
Request::HTTP_CODE Request::parse_content(char* buffer, int& checked_index, CHECK_STATE& checkstate, int& read_index, int& start_line){
    LINE_STATUS linestatus = LINE_OK;
    HTTP_CODE retcode = NO_REQUEST;
    while((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK){
        char* temp = buffer + start_line;
        start_line = checked_index;
        switch(checkstate){
            case CHECK_STATE_REQUESTLINE:
            {
                retcode = parse_requestline(temp, checkstate);
                if(retcode == BAD_REQUEST){
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER:
            {
                retcode = parse_headers(temp);
                if(retcode == BAD_REQUEST){
                    return BAD_REQUEST;
                }
                else if(retcode == GET_REQUEST){
                    return GET_REQUEST;
                }
            }
            default:
            {
                return INTERNAL_ERROR;
            }
        }
    }
    if(linestatus == LINE_OPEN){
        return NO_REQUEST;
    }
    else{
        return BAD_REQUEST;
    }
}
