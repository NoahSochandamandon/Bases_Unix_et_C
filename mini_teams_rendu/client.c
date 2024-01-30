#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Fonction pour envoyer les signaux en fonction du message binaire
void sendSignals(pid_t pid, char c);
int isNumeric(char *str);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <PID cible> <Message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!isNumeric(argv[1]))
    {
        fprintf(stderr, "Erreur : l'argument <PID cible> doit être numérique.\n");
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    char *message = argv[2];

    if (strlen(message) > 1024)
    {
        fprintf(stderr, "Erreur : le <Message> doit faire moins de 1024 charactères\n");
        return EXIT_FAILURE;
    }

    printf("Sending to %d\n", pid);

    int i = 0;
    while (message[i] != '\0')
    {
        sendSignals(pid, message[i]);
        i++;
    }

    sendSignals(pid, '\0');

    printf("Done\n");

    return 0;
}

void sendSignals(pid_t pid, char c)
{
    int i = 7;

    while (i >= 0)
    {
        int bit = (c >> i) & 1;
        if (bit == 1)
        {
            kill(pid, SIGUSR1);
        }
        else
        {
            kill(pid, SIGUSR2);
        }
        usleep(200); // Pause pour assurer la synchronisation
        i--;
    }
    usleep(1000);
}

int isNumeric(char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}