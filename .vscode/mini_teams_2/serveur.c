#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void my_convert_text(char *bin_mes);

void handle_sigusr1(int sig)
{
    putchar('0');
}

void handle_sigusr2(int sig)
{
    putchar('1');
}

int main()
{
    printf("Miniteams starting...\n");
    printf(" My PID is %i\n", getpid());
    printf("Waiting for new messages\n");
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    while (1)
    {
        usleep(100000);
        pause();
    }

    return 0;
}

void my_convert_text(char *bin_mes)
{
    int i = 0;
    int mes_len = strlen(bin_mes);

    while (i < mes_len)
    {
        int b = 0;
        int size = 8;

        while (size >= 1)
        {
            size--;
            b |= ((bin_mes[i] - '0') << size);
            i++;
        }
        putchar((char)b);
    }
}