#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigusr1(int sig)
{
    printf("0\n");
}

void handle_sigusr2(int sig)
{
    printf("1\n");
}

int main()
{
    printf("PID: %d\n", getpid());

    if (signal(SIGUSR1, handle_sigusr1) == SIG_ERR)
    {
        fprintf(stderr, "Impossible de gérer SIGUSR1\n");
        return 1;
    }

    if (signal(SIGUSR2, handle_sigusr2) == SIG_ERR)
    {
        fprintf(stderr, "Impossible de gérer SIGUSR2\n");
        return 1;
    }

    while (1)
    {
        pause();
    }

    return 0;
}