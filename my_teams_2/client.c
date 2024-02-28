// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <IP> <Port> <Pseudo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    const char *pseudo = argv[3];
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connection to server… OK.\n");

    // Send pseudo
    send(sock, pseudo, strlen(pseudo), 0); // Assuming send() is successful for simplicity

    // Set socket to non-blocking mode
    fcntl(sock, F_SETFL, O_NONBLOCK);

    printf("Welcome to the chat server!\n");

    // Communication loop
    fd_set read_fds;
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(sock, &read_fds);

        if (select(sock + 1, &read_fds, NULL, NULL, NULL) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            fgets(buffer, BUFFER_SIZE, stdin);
            if (send(sock, buffer, strlen(buffer), 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
        }

        if (FD_ISSET(sock, &read_fds))
        {
            int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            else if (bytes_received == 0)
            {
                printf("Server closed the connection.\n");
                break;
            }
            else
            {
                buffer[bytes_received] = '\0';
                printf("%s\n", buffer);
            }
        }
    }

    close(sock);
    return 0;
}
