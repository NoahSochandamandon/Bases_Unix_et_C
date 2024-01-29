#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void sendBit(pid_t pid, char bit);
void sendChar(pid_t pid, char c);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <PID cible> <Message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = atoi(argv[1]);
    char *message = argv[2];

    size_t i = 0;
    while (i < strlen(message))
    {
        sendChar(pid, message[i]);
        i++;
    }
    i = 0;
    while (i < 1)
    {
        sendChar(pid, '\n');
        i++;
    }

    return EXIT_SUCCESS;
}

void sendChar(pid_t pid, char c)
{
    for (int i = 7; i >= 0; --i)
    {
        sendBit(pid, (c & (1 << i)) ? '1' : '0');
    }
}

void sendBit(pid_t pid, char bit)
{
    int signal = bit == '1' ? SIGUSR1 : SIGUSR2;
    if (kill(pid, signal) == -1)
    {
        perror("Erreur d'envoi de signal");
        exit(EXIT_FAILURE);
    }
    usleep(1000); // Petite pause pour s'assurer que le signal est reçu
}