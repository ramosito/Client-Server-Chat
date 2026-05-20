#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "common.h"

void receive_messages_loop(int client_socket)
{
    char message[1024];

    while (1) {
        receive_message(client_socket, message, sizeof(message));

        if (message[0] == '\0') {
            break;
        }

        printf("Message reçu : %s\n", message);
    }
}

void* receive_messages_loop_thread(void* arg)
{
    int client_socket = *(int*)arg;
    receive_messages_loop(client_socket);
    return NULL;
}

void send_message_loop(int client_socket)
{
    char message[1024];

    while (1) {
        fgets(message, sizeof(message), stdin);
        send_message(client_socket, message);
    }
}

int main(void)
{
    // Création du socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Définition de l'adresse IP et du port du serveur
    struct sockaddr_in address;
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port        = htons(8989);

    // Connexion au serveur
    int result = connect(client_socket, (struct sockaddr*)&address, sizeof(address));
    if (result == 0) {
        printf("Client connected!\n");
    } else {
        perror("Failed to connect");
        exit(EXIT_FAILURE);
    }

    // Thread pour recevoir les messages du serveur
    pthread_t thread;
    pthread_create(&thread, NULL, receive_messages_loop_thread, &client_socket);

    // Boucle d'envoi des messages
    send_message_loop(client_socket);

    return 0;
}