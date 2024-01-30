#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Fonction pour envoyer les signaux en fonction du message binaire
void sendSignals(pid_t pid, char c) {
    for (int i = 7; i >= 0; --i) {
        int bit = (c >> i) & 1;
        if (bit == 1) {
            kill(pid, SIGUSR1);
        } else {
            kill(pid, SIGUSR2);
        }
        usleep(1000); // Pause pour assurer la synchronisation
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <Message>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    char *message = argv[2];

    // Envoie du message
    for (int i = 0; message[i] != '\0'; ++i) {
        sendSignals(pid, message[i]);
    }

    // Envoie du caractère nul pour indiquer la fin du message
    sendSignals(pid, '\0');

    return 0;
}

