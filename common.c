#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "common.h"

int send_message(int socket, char *message) {
    if (send(socket, message, strlen(message), 0) < 0) {
        perror("Error sending message");
        return -1;
    }

    return 0;
}

int receive_message(int socket, char *message, size_t max_length) {
    int receive_length = recv(socket, message, max_length, 0);
    if (receive_length == 0) {
        printf("Peer disconnected\n");
        return -1;
    }
    else if (receive_length < 0) {
        perror("Error receiveing message");
        return -1;
    }

    if (message[receive_length - 1] == '\n') {
        message[receive_length - 1] = '\0';
    }
    else {
        message[receive_length] = '\0';
    }

    return 0;
}
