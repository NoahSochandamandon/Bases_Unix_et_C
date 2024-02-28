je vais te donner 2 scripts, qui fonctionne ensemblre, enregistre les et garde les en mémoire en attendant mes prochaines instruction.

Script Serveur :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <errno.h> // Inclus pour gestion des erreurs

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
                buffer[valread] = '\0'; // Assurez-vous que le pseudo est une chaîne terminée par NULL
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
```

Script Client :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define PSEUDO_SIZE 32

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <IP> <Port> <Pseudo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    fd_set readfds;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("Connection to server… OK.\n");

    // Envoi du pseudo au serveur
    send(sock, argv[3], strlen(argv[3]), 0);

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        select(sock + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(sock, &readfds))
        {
            valread = read(sock, buffer, BUFFER_SIZE);
            if (valread > 0)
            {
                buffer[valread] = '\0';
                printf("%s\n", buffer);
                // Déplace l'invite ici pour s'assurer qu'elle s'affiche après la réception du message
                printf("Send a new message: ");
            }
        }

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Supprime le retour à la ligne

            char messageToSend[BUFFER_SIZE] = {0};
            strncpy(messageToSend, argv[3], PSEUDO_SIZE - 1);
            strcat(messageToSend, ": ");
            size_t remainingSpace = BUFFER_SIZE - strlen(messageToSend) - 1;
            strncat(messageToSend, buffer, remainingSpace);

            send(sock, messageToSend, strlen(messageToSend), 0);
            // Ne pas afficher l'invite ici, elle sera affichée après la réception d'une réponse du serveur
        }
    }

    return 0;
}
```
