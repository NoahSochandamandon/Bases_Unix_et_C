#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void fin(int sig);

int main(int argc, char const *argv[])
{
    printf("PID: %d\n", getpid());

    signal(SIGINT, fin);

    pause();

    return 0;
}

void fin(int sig)
{
    printf("Le programme s'arrete\n");
}