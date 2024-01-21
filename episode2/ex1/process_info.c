#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n", getpgrp());

    return 0;
}