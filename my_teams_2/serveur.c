// serveur.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define PSEUDO_SIZE 100
#define MESSAGE_SIZE (PSEUDO_SIZE + BUFFER_SIZE + 10) // Pseudo + " > " + Buffer + '\0'

// Structure to store client information
typedef struct
{
    int sock;
    char pseudo[PSEUDO_SIZE];
} Client;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_port = atoi(argv[1]);
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    Client clients[MAX_CLIENTS] = {{0}};
    int activity, valread, sd;
    int max_sd;
    fd_set readfds;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server socket to allow multiple connections
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server_port);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Binding… OK.\nServer reachable on IP 0.0.0.0 port %d\nWaiting for new clients…\n", server_port);

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = clients[i].sock;
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
            int new_socket;
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, ip is : %s, port : %d \n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (clients[i].sock == 0)
                {
                    clients[i].sock = new_socket;
                    valread = recv(new_socket, clients[i].pseudo, PSEUDO_SIZE, 0);
                    if (valread < 0)
                    {
                        perror("recv");
                        close(new_socket);
                        clients[i].sock = 0;
                    }
                    else
                    {
                        clients[i].pseudo[valread] = '\0'; // Null-terminate the pseudo string
                        printf("Adding to list of sockets as %d with pseudo %s\n", i, clients[i].pseudo);
                    }
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = clients[i].sock;

            if (FD_ISSET(sd, &readfds))
            {
                valread = recv(sd, buffer, BUFFER_SIZE, 0);
                if (valread == 0)
                {
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                    printf("Host disconnected, ip %s, port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    close(sd);
                    clients[i].sock = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    char message[MESSAGE_SIZE];
                    snprintf(message, sizeof(message), "%s > %s", clients[i].pseudo, buffer);
                    for (int j = 0; j < MAX_CLIENTS; j++)
                    {
                        if (clients[j].sock != 0 && clients[j].sock != sd)
                        {
                            send(clients[j].sock, message, strlen(message), 0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
