/*
Exercise 1-19. Write a program detab which replaces tabs in the input with the
proper number of blanks to space to the next tab stop. Assume a fixed set of tab
stops, say every n positions.
 */
#include <stdio.h>

#define TABSTOP 4

int main(void) {

    char c;
    int pos = 1;
    int nb;

    while ((c = getchar()) != EOF) {
        if (c == '\t')
            for (nb = TABSTOP - (pos - 1) % TABSTOP; nb > 0; nb--, pos++)
                (void)putchar(' ');
        else {
            (void)putchar(c);
            if (c == '\n')
                pos = 1;
            else
                pos++;
        }
    }

    return 0;
}
