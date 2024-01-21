#include <stdio.h>
int my_putstr(char *str);

int main(void)
{
    char str[] = "j'aime les poneys";

    my_putstr(str);
    return 0;
}

int my_putstr(char *str)
{
    while (*str != '\0')
    {
        putchar(*str);
        *str++;
    }
    putchar('\n');
}