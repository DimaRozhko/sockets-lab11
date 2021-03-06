#include "../../common/constant.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    int sock, listener;
    struct sockaddr_in addr;
    char buf[MESSAGE_LEN];
    int bytes_read;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0){
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }
    listen(listener, 1);
    while(1) {
        sock = accept(listener, NULL, NULL);
        if(sock < 0) {
            perror("accept");
            exit(3);
        }
        switch(fork()){
            case -1:
                perror("fork");
                break;
            case 0:
                close(listener);
                printf("STREAMING ASERVER SINC GOT MESSAGE: ");
                while(1) {
                    bytes_read = recv(sock, buf, 1024, 0);
                    if(bytes_read <= 0) break;
                    printf("%s\n", buf);
                    send(sock, buf, bytes_read, 0);
                }
                close(sock);
                _exit(0);
            default:
                close(sock);
        }
    }
    close(listener);
    return 0;
}