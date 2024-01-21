#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

void fin(int sig);

int main(int argc, char const *argv[])
{
    printf("PID: %d\n", getpid());

    const int SIG[] = {SIGUSR1, SIGUSR2, SIGINT, SIGQUIT};
    int i = 0;

    while (i < 4)
    {
        signal(SIG[i], fin);
        i++;
    }

    while (1)
    {
        pause();
    }

    return 0;
}

void fin(int sig)
{
    printf("Signal reçu: %d\n", sig);

    // char pid;
    // char *desc = strsignal(sig);

    char *type = "inconnu";

    if (sig == SIGUSR1)
    {
        type = "received SIGUSR1";
    }
    else if (sig == SIGUSR2)
    {
        type = "received SIGUSR2";
    }
    else if (sig == SIGINT)
    {
        type = "received SIGINT";
    }
    else if (sig == SIGQUIT)
    {
        type = "received SIGQUIT";
    }

    printf("[FROM] %s (Signal) %d", type, sig);
}