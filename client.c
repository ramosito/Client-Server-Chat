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
    srand(time(NULL) ^ getpid());

    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nErreur de connexion \n");
        return -1;
    }
    printf("Connecte au serveur !\n\n");

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
    // Le client reçoit d'abord, puis envoie
    recv(sock, &e_distant, sizeof(unsigned long long), 0);
    recv(sock, &n_distant, sizeof(unsigned long long), 0);
    send(sock, &e_local, sizeof(unsigned long long), 0);
    send(sock, &n_local, sizeof(unsigned long long), 0);
    printf("[RSA] Handshake termine. Cles echangees.\n");

    // --- 3. BOUCLE DE CHAT ---
    char buffer[BUFFER_SIZE];
    unsigned long long message_crypto[BUFFER_SIZE];
    int longueur;

    while (1) {
        // ENVOI
        printf("\nClient : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        longueur = strlen(buffer);
        
        for(int i = 0; i < longueur; i++) {
            message_crypto[i] = chiffrer((unsigned long long)buffer[i], e_distant, n_distant);
        }
        send(sock, &longueur, sizeof(int), 0);
        send(sock, message_crypto, longueur * sizeof(unsigned long long), 0);

        // RECEPTION
        int bytes = recv(sock, &longueur, sizeof(int), 0);
        if (bytes <= 0) break;
        
        recv(sock, message_crypto, longueur * sizeof(unsigned long long), 0);
        for(int i = 0; i < longueur; i++) {
            buffer[i] = (char)dechiffrer(message_crypto[i], d_local, n_local);
        }
        buffer[longueur] = '\0';
        printf("Serveur : %s", buffer);
    }

    close(sock);
    return 0;
}