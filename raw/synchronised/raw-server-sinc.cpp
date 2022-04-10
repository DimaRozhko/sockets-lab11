#include "../../common/constant.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main() {
    int listener = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
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
    char* read_buffer;
    int bytes_read = 0;
   
    while (1) {
        read_buffer = (char*)malloc(MESSAGE_LEN * sizeof(char));
        socklen_t address_len = sizeof(address);
        bytes_read = recvfrom(listener, read_buffer, MESSAGE_LEN, 
            MSG_CONFIRM, (struct sockaddr *) &address, &address_len);
        if (bytes_read <= 0) {
            break;
        }
        printf("RAW SERVER SINC GOT MESSAGE: ");
        printf("%s\n", read_buffer);
        free(read_buffer);
    }
    close(listener);
    return 0;
}