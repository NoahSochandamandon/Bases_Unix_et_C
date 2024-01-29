#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

char binaryMessage[MAX_MESSAGE_LENGTH];
int binaryIndex = 0;
int messageComplete = 0;

void handleSIGUSR1();
void handleSIGUSR2();
void handleSIGALRM();
void binaryToText(char *binary, char *text);

int main()
{
    printf("PID du Serveur: %d\n", getpid());
    fflush(stdout);

    struct sigaction sa1, sa2, sa3;
    memset(&sa1, 0, sizeof(sa1));
    sa1.sa_handler = handleSIGUSR1;
    sigaction(SIGUSR1, &sa1, NULL);

    memset(&sa2, 0, sizeof(sa2));
    sa2.sa_handler = handleSIGUSR2;
    sigaction(SIGUSR2, &sa2, NULL);

    memset(&sa3, 0, sizeof(sa3));
    sa3.sa_handler = handleSIGALRM;
    sigaction(SIGALRM, &sa3, NULL);

    while (!messageComplete)
    {
        pause();

        if (messageComplete)
        {
            binaryMessage[binaryIndex] = '\0'; // Fin du message

            char textMessage[MAX_MESSAGE_LENGTH];
            binaryToText(binaryMessage, textMessage);

            time_t currentTime;
            time(&currentTime);
            struct tm *localTime = localtime(&currentTime);

            printf("[%02d:%02d] %s\n", localTime->tm_hour, localTime->tm_min, textMessage);

            memset(binaryMessage, 0, MAX_MESSAGE_LENGTH);
            binaryIndex = 0;
            messageComplete = 0;
        }
    }

    return 0;
}

void handleSIGUSR1()
{
    binaryMessage[binaryIndex++] = '1';
}

void handleSIGUSR2()
{
    binaryMessage[binaryIndex++] = '0';
}

void handleSIGALRM()
{
    messageComplete = 1;
}

void binaryToText(char *binary, char *text)
{
    while (*binary)
    {
        char byte = 0;
        for (int i = 0; i < 8 && *binary; ++i)
        {
            byte = (byte << 1) | (*binary++ - '0');
        }
        *text++ = byte;
    }
    *text = '\0';
}