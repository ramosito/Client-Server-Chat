#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "common.h"
#include "client_array.h"

struct ThreadData {
    int client_socket;
    struct ClientArray* client_array;
};

void receive_messages_loop(int client_socket, struct ClientArray* client_array)
{
    char message[1024];

    while (1) {
        receive_message(client_socket, message, sizeof(message));

        // Si message vide, le client s'est déconnecté
        if (message[0] == '\0') {
            break;
        }

        printf("Message reçu : %s\n", message);

        // Renvoyer le message à tous les clients
        for (int i = 0; i < client_array->size; i++) {
            send_message(client_array->clients[i].socket, message);
        }
    }
}

void* receive_messages_thread(void* arg)
{
    struct ThreadData* data = (struct ThreadData*)arg;
    receive_messages_loop(data->client_socket, data->client_array);
    return NULL;
}

int main(void)
{
    // Création du socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    const int ON = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &ON, sizeof(ON)) < 0) {
        perror("Failed setting socket options");
        exit(EXIT_FAILURE);
    }

    // Définition de l'adresse et du port
    struct sockaddr_in address;
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(8989);

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8989...\n");

    // Tableau des clients connectés
    struct ClientArray client_array = ca_init();

    // Boucle infinie d'acceptation des clients
    while (1) {
        struct sockaddr_in client_address;
        unsigned int client_address_len = sizeof(client_address);

        int client_socket = accept(server_socket,
                                   (struct sockaddr*)&client_address,
                                   &client_address_len);
        if (client_socket >= 0) {
            printf("A client is connected!\n");

            // Création du thread pour ce client
            struct Client new_client;
            new_client.socket = client_socket;

            struct ThreadData* data = malloc(sizeof(struct ThreadData));
            data->client_socket = client_socket;
            data->client_array  = &client_array;

            pthread_create(&new_client.thread, NULL, receive_messages_thread, data);

            // Ajout du client dans le tableau
            ca_add(&client_array, new_client);
        } else {
            perror("Error accepting client connection");
        }
    }

    return 0;
}