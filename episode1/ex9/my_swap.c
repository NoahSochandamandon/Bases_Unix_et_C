#include <stdio.h>
void my_swap(int *a, int *b);

int main(void)
{
    int my_int1;
    int my_int2;

    my_int1 = 42;
    my_int2 = 84;

    printf("Value of int1 = % i \n", my_int1);
    printf("Value of int2 = % i \n", my_int2);
    my_swap(&my_int1, &my_int2);
    printf("Value of int1 = % i \n", my_int1);
    printf("Value of int2 = % i \n", my_int2);

    return 0;
}

void my_swap(int *a, int *b)
{
    int temporaire;

    temporaire = *a;
    *a = *b;
    *b = temporaire;
}