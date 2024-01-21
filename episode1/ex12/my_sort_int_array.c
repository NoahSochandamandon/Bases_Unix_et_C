#include <stdio.h>
void my_sort_int_array(int *tab, int size);

int main(void)
{
    int tab[5] = {64, 58, -52, 42, 5};

    my_sort_int_array(tab, 5);
    printf("%d \n", tab[0]);
    printf("%d \n", tab[4]);
}

void my_sort_int_array(int *tab, int size)
{
    int changement = 1;
    int i = 0;
    int temp;

    while (changement != 0)
    {
        changement = 0;
        i = 0;

        while (i < size - 1)
        {
            if (i < size - 1 && tab[i] > tab[i + 1])
            {
                temp = tab[i];
                tab[i] = tab[i + 1];
                tab[i + 1] = temp;
                changement++;
            }
            i++;
        }
    }
}