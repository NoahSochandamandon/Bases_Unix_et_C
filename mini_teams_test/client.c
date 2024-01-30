#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// Fonction pour envoyer les signaux en fonction du message binaire
void sendSignals(pid_t pid, char c);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <PID cible> <Message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = atoi(argv[1]);
    char *message = argv[2];

    int i = 0;
    while (message[i] != '\0')
    {
        sendSignals(pid, message[i]);
        i++;
    }

    // Envoie du signal SIGALRM pour indiquer la fin du message
    kill(pid, SIGALRM);

    return 0;
}

void sendSignals(pid_t pid, char c)
{
    for (int i = 7; i >= 0; --i)
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
        usleep(500); // Pause pour assurer la synchronisation
    }
}