#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("PID: %d\n", getpid());
    while (1)
    {
        printf("Oui bien\n");
        sleep(2);
    }
}