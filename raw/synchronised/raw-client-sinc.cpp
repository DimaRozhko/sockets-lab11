#include "../../common/constant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char* argv[]) {
    int socket_client = socket(AF_INET, SOCK_RAW,0);
    if (socket_client < 0) {
        perror("ERROR: incorrect socket\n");
        exit(1);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(socket_client, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("ERROR: can't connect to server\n");
        exit(2);
    }
    char message[MESSAGE_LEN];
    strcpy(message, argc <= 1 ? "Raw sinchronised message" : argv[1]);
    send(socket_client, message, MESSAGE_LEN, 0);
    char recvest[MESSAGE_LEN];
    recv(socket_client, recvest, sizeof(MESSAGE_LEN), 0);
    close(socket_client);
    return 0;
}