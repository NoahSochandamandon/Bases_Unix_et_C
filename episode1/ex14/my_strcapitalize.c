#include <stdio.h>

void *my_strcapitalize(char* str);

int main(void)
{
    char *str[] = "salam mon reuf, comment va ? Calcul : 42 + (-5) = 37;";

    my_strcapitalize(str);
    my_putstr(str);
}

void *my_strcapitalize(char* str)
{
    
}