#include <stdio.h>
void my_isneg(int n);

int main(void)
{
    int pos;
    int neg;

    pos = 42;
    neg = -84;

    my_isneg(pos);
    my_isneg(21);
    my_isneg(neg);

    return 0;
}

void my_isneg(int n)
{
    if (n < 0)
        printf("ne nombre est négatif \n");
    else
        printf("le nombre est positif \n");
}