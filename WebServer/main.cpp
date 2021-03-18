#include "WebServer.h"
#define BUFFER_SIZE 4096

using namespace std;

int main(int argc, char* argv[])
{
    if(argc <= 2){
        printf("...");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    WebServer webserver{ip, port};
    webserver.await();

    return 0;
}
