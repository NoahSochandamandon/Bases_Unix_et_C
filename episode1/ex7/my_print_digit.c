#include <stdio.h>
int my_print_digit(void); // permet d'éviter les warning  de compilation

int main(void)
{
    my_print_digit();
    return 0;
}

int my_print_digit(void)
{
    int compteur = 0;
    while (compteur < 10)
    {
        putchar('0' + compteur);
        compteur++;
    }
    putchar('\n');
}