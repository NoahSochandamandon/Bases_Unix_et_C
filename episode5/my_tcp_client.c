#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <IP> <Port> <Message>\n", argv[0]);
        return 1;
    }

    // Créer une socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    // Définir l'adresse du serveur
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2])); // Convertir le numéro de port en entier puis en format réseau
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    // Se connecter au serveur
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect");
        close(sock);
        return 1;
    }

    // Envoyer le message
    if (write(sock, argv[3], strlen(argv[3])) < 0)
    {
        perror("write");
        close(sock);
        return 1;
    }

    printf("Message envoyé au serveur : %s\n", argv[3]);

    // Fermer la socket
    close(sock);
    return 0;
}
