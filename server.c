#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "rsa_crypto.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Initialisation aléatoire basée sur le PID pour éviter d'avoir les mêmes clés que le client
    srand(time(NULL) ^ getpid());

    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    printf("Serveur en ecoute sur le port %d...\n", PORT);
    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Client connecte !\n\n");

    // --- 1. GENERATION DES CLES ---
    unsigned long long p = generer_petit_premier();
    unsigned long long q = generer_petit_premier();
    while(p == q) q = generer_petit_premier();
    
    unsigned long long n_local = p * q;
    unsigned long long phi = (p - 1) * (q - 1);
    unsigned long long e_local = calculer_e(phi);
    unsigned long long d_local = calculer_d(e_local, phi);
    
    unsigned long long e_distant, n_distant;

    // --- 2. HANDSHAKE RSA ---
    send(client_socket, &e_local, sizeof(unsigned long long), 0);
    send(client_socket, &n_local, sizeof(unsigned long long), 0);
    recv(client_socket, &e_distant, sizeof(unsigned long long), 0);
    recv(client_socket, &n_distant, sizeof(unsigned long long), 0);
    printf("[RSA] Handshake termine. Cles echangees.\n");

    // --- 3. BOUCLE DE CHAT ---
    char buffer[BUFFER_SIZE];
    unsigned long long message_crypto[BUFFER_SIZE];
    int longueur;

    while (1) {
        // RECEPTION
        int bytes = recv(client_socket, &longueur, sizeof(int), 0);
        if (bytes <= 0) break;
        
        recv(client_socket, message_crypto, longueur * sizeof(unsigned long long), 0);
        for(int i = 0; i < longueur; i++) {
            buffer[i] = (char)dechiffrer(message_crypto[i], d_local, n_local);
        }
        buffer[longueur] = '\0';
        printf("\nClient : %s", buffer);

        // ENVOI
        printf("Serveur : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        longueur = strlen(buffer);
        
        for(int i = 0; i < longueur; i++) {
            message_crypto[i] = chiffrer((unsigned long long)buffer[i], e_distant, n_distant);
        }
        send(client_socket, &longueur, sizeof(int), 0);
        send(client_socket, message_crypto, longueur * sizeof(unsigned long long), 0);
    }

    close(client_socket);
    close(server_fd);
    return 0;
}