#include <stdio.h>
void my_print_revalpha(void);

int main(void)
{
    my_print_revalpha();
    putchar('\n');
    return 0;
}

void my_print_revalpha(void)
{
    int compteur = 0;
    while (compteur < 26)
    {
        putchar('Z' - compteur);
        compteur++;
    }
}