#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1000
#define BYTE 8

char binaryChar[BYTE];
int binaryIndex = 0;
char message[MAX_MESSAGE_LENGTH];
int messageIndex = 0;
pid_t clientPid = 0;

// Handler pour SIGUSR1
void handleSIGUSR1() {
    if (binaryIndex < BYTE) {
        binaryChar[binaryIndex++] = '1';
    }
}

// Handler pour SIGUSR2
void handleSIGUSR2() {
    if (binaryIndex < BYTE) {
        binaryChar[binaryIndex++] = '0';
    }
}

// Convertit un caractère binaire en caractère ASCII
char binaryToASCII(char *binary) {
    char result = 0;
    for (int i = 0; i < BYTE; ++i) {
        result = (result << 1) | (binary[i] - '0');
    }
    return result;
}

void saveMessage(const char *message) {
    FILE *file = fopen("LogsServer.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}

int main() {
    printf("PID du Serveur: %d\n", getpid());

    struct sigaction sa1, sa2;
    memset(&sa1, 0, sizeof(sa1));
    sa1.sa_handler = handleSIGUSR1;
    sigaction(SIGUSR1, &sa1, NULL);

    memset(&sa2, 0, sizeof(sa2));
    sa2.sa_handler = handleSIGUSR2;
    sigaction(SIGUSR2, &sa2, NULL);

    while (1) {
        pause();

        if (binaryIndex == BYTE) {
            char asciiChar = binaryToASCII(binaryChar);

            if (asciiChar == '\0') {
                // Fin du message
                message[messageIndex] = '\0';

                // Traitement du message reçu
                char formattedMessage[MAX_MESSAGE_LENGTH + 50];
                sprintf(formattedMessage, "Message de %d: %s\n", clientPid, message);
                saveMessage(formattedMessage);
                printf("%s", formattedMessage);

                // Réinitialiser pour le prochain message
                memset(message, 0, MAX_MESSAGE_LENGTH);
                messageIndex = 0;
                clientPid = 0;
            } else {
                // Ajout du caractère au message
                if (messageIndex < MAX_MESSAGE_LENGTH) {
                    message[messageIndex++] = asciiChar;
                }
            }

            // Réinitialiser pour le prochain caractère
            memset(binaryChar, 0, BYTE);
            binaryIndex = 0;
        }
    }

    return 0;
}

