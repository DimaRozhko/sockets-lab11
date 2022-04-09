#include "../../common/constant.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main() {
    int listener = socket(AF_INET, SOCK_RAW,0);
    if (listener < 0) {
        perror("ERROR: incorrect socket\n");
        exit(1);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("ERROR: can't bind\n");
        exit(2);
    }
    listen(listener, 0);
    int server_socket;
    char read_buffer[MESSAGE_LEN];
    int bytes_read = 0;
   
    while (1) {
        server_socket = accept(listener, NULL, NULL);
        if(server_socket < 0){
            perror("ERROR: can't accept");
            exit(3);
        }
        printf("STREAMING SERVER SINC GOT MESSAGE: ");
        while (1) {
            bytes_read = recv(server_socket, read_buffer, MESSAGE_LEN, 0);
            if (bytes_read <= 0) {
                break;
            }
            printf("%s\n", read_buffer);
            send(server_socket, read_buffer, bytes_read, 0);
        }
        close(server_socket);
    }
    return 0;
}