#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 30
#define PSEUDO_SIZE 32

int client_socket[MAX_CLIENTS];
char client_pseudo[MAX_CLIENTS][PSEUDO_SIZE];

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, new_socket, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    fd_set readfds;

    for (i = 0; i < MAX_CLIENTS; i++)
    {
        client_socket[i] = 0;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
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
    printf("Binding... OK.\n");

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server reachable on IP 0.0.0.0 port %s\n", argv[1]);
    printf("Waiting for new clients...\n");

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        if (FD_ISSET(server_fd, &readfds))
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // Lecture du pseudo du client
            memset(buffer, 0, BUFFER_SIZE);
            valread = read(new_socket, buffer, PSEUDO_SIZE - 1);
            if (valread > 0)
            {
                buffer[valread] = '\0'; // Vérification que le pseudo est une chaîne terminée par NULL
                printf("New connection - socket fd is %d, ip is: %s, port: %d, pseudo: %s\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port), buffer);

                for (i = 0; i < MAX_CLIENTS; i++)
                {
                    if (client_socket[i] == 0)
                    {
                        client_socket[i] = new_socket;
                        strncpy(client_pseudo[i], buffer, PSEUDO_SIZE - 1);
                        printf("Adding to list of sockets as %d\n", i);
                        break;
                    }
                }
            }
        }

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
                valread = read(sd, buffer, BUFFER_SIZE - 1);
                if (valread == 0)
                {
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                    printf("Host disconnected, ip %s, port %d, pseudo %s\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port), client_pseudo[i]);

                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    // Diffusion du message à tous les clients
                    for (int j = 0; j < MAX_CLIENTS; j++)
                    {
                        if (client_socket[j] != 0)
                        {
                            write(client_socket[j], buffer, strlen(buffer));
                        }
                    }
                }
            }
        }
    }

    return 0;
}
