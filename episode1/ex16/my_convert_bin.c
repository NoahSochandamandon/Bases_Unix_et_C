#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void my_putstr(char *str);
void my_convert_bin(char *mot);

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int length = strlen(argv[1]);
    char *mot = malloc(length + 1);

    if (mot == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        return 1;
    }

    strcpy(mot, argv[1]);

    my_convert_bin(mot);

    return 0;
}

void my_convert_bin(char *mot)
{

    int placement_bin;
    int bit;
    int lettre = 0;

    while (mot[lettre] != '\0')
    {
        bit = 7;
        char lettre_bin[9] = "";

        while (bit >= 0)
        {
            lettre_bin[7 - bit] = (mot[lettre] & (1 << bit)) ? '1' : '0';
            bit--;
        }

        lettre_bin[8] = '\0';

        my_putstr(lettre_bin);

        lettre++;
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