#include <stdio.h>
char *my_strcpy(char *dest, char const *src);
void my_putstr(char *str);

int main(void)
{
    char src[256] = "hello hello";
    char dest[256];

    my_putstr(src);
    my_strcpy(dest, src);
    my_putstr(dest);

    return 0;
}

char *my_strcpy(char *dest, char const *src)
{
    int compteur = 0;

    while(src[compteur] != '\0')
    {
        dest[compteur] = src[compteur];
        compteur ++;
    }
}

void my_putstr(char *str)
{
    while (*str != '\0')
    {
        putchar(*str);
        *str++;
    }
    putchar('\n');
}