#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h> // Pour fcntl
#include <errno.h> // Pour errno et les codes d'erreur

#define BUFFER_SIZE 1024

void setSocketNonBlocking(int socket_fd)
{
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        return 1;
    }

    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    setSocketNonBlocking(server_fd); // Rendre la socket serveur non bloquante

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

    printf("Listening on port %s\n", argv[1]);
    printf("Waiting for connections...\n");

    while (1)
    {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            usleep(100000); // Attente pour éviter une utilisation élevée du CPU
            continue;
        }

        setSocketNonBlocking(client_socket); // Rendre la socket client non bloquante

        printf("Client connected\n");

        while (1)
        {
            memset(buffer, 0, BUFFER_SIZE);
            int valread = read(client_socket, buffer, BUFFER_SIZE - 1);
            if (valread > 0)
            {
                printf("Received => %s\n", buffer);
            }
            else if (valread == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                // Pas de données disponibles pour le moment, continuez à tourner
                sleep(100000); // Attente pour réduire l'utilisation du CPU
            }
            else
            {
                // Erreur de lecture ou connexion fermée par le client
                break;
            }
        }

        printf("Client disconnected\n");
        close(client_socket);
    }

    // La socket serveur ne sera jamais fermée dans ce code, mais vous pourriez ajouter une gestion de signal pour le faire proprement
    return 0;
}
