#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define MAX_BITS 8
#define MAX_MESSAGE_LENGTH 1024

static int bitsReceived = 0;
static char currentChar = 0;
static char message[MAX_MESSAGE_LENGTH];
static int messageIndex = 0;

void handleSignal(int sig);

int main()
{
    printf("PID: %d\n", getpid());

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handleSignal;
    sigfillset(&sa.sa_mask);  // Bloque les autres signaux pendant l'exécution du gestionnaire
    sa.sa_flags = SA_RESTART; // Redémarre les fonctions si interrompues par un appel système

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    memset(message, 0, sizeof(message)); // Initialise le buffer de message

    while (1)
    {
        pause(); // Attend le signal indéfiniment

        // A chaque signal reçu, vérifie si un caractère a été complètement reçu et affiche le message en cours
        if (bitsReceived == 0 && messageIndex > 0)
        {
            printf("%s", message);            // Affiche le message reçu jusqu'à présent
            fflush(stdout);                   // S'assure que le message est immédiatement affiché
            memset(message, 0, messageIndex); // Efface le buffer après l'affichage
            messageIndex = 0;                 // Réinitialise l'index pour le prochain message
        }
    }

    return EXIT_SUCCESS;
}

void handleSignal(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
    }

    // Ajoute un bit au caractère courant
    if (sig == SIGUSR1)
    {
        currentChar |= (1 << (MAX_BITS - 1 - bitsReceived));
    }
    bitsReceived++;

    // Vérifie si tous les bits du caractère courant ont été reçus
    if (bitsReceived == MAX_BITS)
    {
        message[messageIndex++] = currentChar; // Ajoute le caractère au message
        if (messageIndex < MAX_MESSAGE_LENGTH - 1)
        {
            message[messageIndex] = '\0'; // S'assure que le message est toujours terminé correctement
        }
        else
        {
            // Évite le débordement du buffer
            fprintf(stderr, "Erreur : le message est trop long.\n");
            exit(EXIT_FAILURE);
        }

        // Réinitialise pour le prochain caractère
        bitsReceived = 0;
        currentChar = 0;
    }
}