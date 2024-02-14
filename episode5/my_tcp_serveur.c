#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        return 1;
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Créer une socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attacher la socket au port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %s\n", argv[1]);
    printf("En attente de connexions...\n");

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Obtention de l'adresse IP et du port du client
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        if (getpeername(new_socket, (struct sockaddr *)&client_addr, &client_len) == -1)
        {
            perror("getpeername failed");
            close(new_socket);
            continue; // Passe au prochain client en cas d'échec
        }

        char *client_ip = inet_ntoa(client_addr.sin_addr);
        int client_port = ntohs(client_addr.sin_port);

        // int valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("Reçu de %s:%d => %s\n", client_ip, client_port, buffer);
        close(new_socket); // Fermer la socket de connexion avec le client actuel

        // Réinitialiser le buffer
        memset(buffer, 0, BUFFER_SIZE);
    }

    // close(server_fd); // Cette ligne ne sera jamais atteinte à cause de la boucle infinie
    return 0;
}
