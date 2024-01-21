# include <stdio.h>

int main() {
    char letter = 'X';
    int interator = 0;
    while (interator < 42) {
        my_print_char(letter);
        interator ++;
    }
    my_print_char('\n');
    printf("J'ai ecrit %i fois %c \n", interator, letter);

}

void my_print_char(char c) {
    putchar(c);
}

