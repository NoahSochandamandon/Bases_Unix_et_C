#include <stdio.h>
int my_strlen(char *str);

int main(void)
{
    char str[] = "j'aime les poneys";
    int size;

    size = my_strlen(str);
    printf("la taille de la chaine de charactere est de : %d \n", size);
    return 0;
}

int my_strlen(char *str)
{
    int taille = 0;
    while (*str != '\0')
    {
        *str++;
        taille++;
    }
    return taille;
}