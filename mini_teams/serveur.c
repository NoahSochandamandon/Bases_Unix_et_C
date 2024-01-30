#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024
#define BYTE 8

char binaryChar[BYTE];
int binaryIndex = 0;
char message[MAX_MESSAGE_LENGTH];
int messageIndex = 0;
pid_t clientPid = 0;

void charToText(char letter);
void handleSIGUSR1(int sig __attribute__((unused)), siginfo_t *si, void *context __attribute__((unused)));
void handleSIGUSR2(int sig __attribute__((unused)), siginfo_t *si, void *context __attribute__((unused)));
void saveMessage(const char *message);
char binaryToASCII(char *binary);

int main()
{
    printf("Miniteams starting...\n");
    printf("My PID is %d\n", getpid());
    printf("Waiting for new messages\n");
    fflush(stdout);

    struct sigaction sa1, sa2;

    memset(&sa1, 0, sizeof(sa1));
    sa1.sa_sigaction = handleSIGUSR1;
    sa1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa1, NULL);

    memset(&sa2, 0, sizeof(sa2));
    sa2.sa_sigaction = handleSIGUSR2;
    sa1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &sa2, NULL);

    while (1)
    {
        pause();

        if (binaryIndex == BYTE)
        {
            char asciiChar = binaryToASCII(binaryChar);

            if (asciiChar == '\0')
            {
                // Fin du message
                message[messageIndex] = '\0';

                time_t currentTime;
                time(&currentTime);
                struct tm *localTime = localtime(&currentTime);

                // Traitement du message reçu
                char formattedMessage[MAX_MESSAGE_LENGTH + 64];
                sprintf(formattedMessage, "[%02d:%02d:%02d] Message de %d: %s\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec, clientPid, message);
                saveMessage(formattedMessage);
                printf("%s", formattedMessage);

                // Réinitialiser pour le prochain message
                memset(message, 0, MAX_MESSAGE_LENGTH);
                messageIndex = 0;
                clientPid = 0;
            }

            else
            {
                // Ajout du caractère au message
                if (messageIndex < MAX_MESSAGE_LENGTH)
                {
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

char binaryToASCII(char *binary)
{
    char result = 0;
    for (int i = 0; i < BYTE; ++i)
    {
        result = (result << 1) | (binary[i] - '0');
    }
    return result;
}

void handleSIGUSR1(int sig __attribute__((unused)), siginfo_t *si, void *context __attribute__((unused)))
{
    if (clientPid == 0)
    {
        clientPid = si->si_pid;
    }

    if (binaryIndex < BYTE)
    {
        binaryChar[binaryIndex++] = '1';
    }
}

void handleSIGUSR2(int sig __attribute__((unused)), siginfo_t *si, void *context __attribute__((unused)))
{
    if (clientPid == 0)
    {
        clientPid = si->si_pid;
    }

    if (binaryIndex < BYTE)
    {
        binaryChar[binaryIndex++] = '0';
    }
}

void saveMessage(const char *message)
{
    FILE *file = fopen("logs.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}