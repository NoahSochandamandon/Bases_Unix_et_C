#include <stdio.h>
void my_print_alpha(void);

int main(void)
{
    my_print_alpha();
    putchar('\n');
    return 0;
}

void my_print_alpha(void)
{
    int compteur = 0;
    while (compteur < 26)
    {
        putchar('A' + compteur);
        compteur++;
    }
}