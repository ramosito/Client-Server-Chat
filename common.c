#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void send_message(int socket, char* message)
{
    if (send(socket, message, strlen(message), 0) < 0) {
        perror("Failed to send message");
        exit(EXIT_FAILURE);
    }
}

void receive_message(int socket, char* message, size_t max_length)
{
    int bytes_read = recv(socket, message, max_length - 1, 0);

    if (bytes_read == 0) {
        printf("Peer disconnected\n");
        message[0] = '\0';
        return;
    }

    if (bytes_read < 0) {
        perror("Failed to receive message");
        exit(EXIT_FAILURE);
    }

    message[bytes_read] = '\0';

    // Supprime le saut de ligne final s'il existe
    if (bytes_read > 0 && message[bytes_read - 1] == '\n') {
        message[bytes_read - 1] = '\0';
    }
}