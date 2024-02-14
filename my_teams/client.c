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
        FD_SET(0, &readfds);

        select(sock + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(sock, &readfds))
        {
            valread = read(sock, buffer, BUFFER_SIZE);
            if (valread > 0)
            {
                buffer[valread] = '\0';
                printf("%s\n", buffer);
            }
        }

        if (FD_ISSET(0, &readfds))
        {
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Supprime le retour à la ligne

            char messageToSend[BUFFER_SIZE] = {0}; // Assurez-vous que le tampon est vide
            strncpy(messageToSend, argv[3], PSEUDO_SIZE - 1);
            strcat(messageToSend, ": "); // Utilisation de strcat pour ajouter ": "

            // Calcule l'espace restant après l'ajout du pseudo et de ": "
            size_t remainingSpace = BUFFER_SIZE - strlen(messageToSend) - 1;
            strncat(messageToSend, buffer, remainingSpace);

            send(sock, messageToSend, strlen(messageToSend), 0);
            printf("Send a new message: ");
        }
    }

    return 0;
}
