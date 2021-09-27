/*
Exercise 1-13. Write a program to convert its input to lower case, using a
function lower(c) which returns c if c is not a letter, and the lower case value
of c if it is a letter.
 */
 #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *toLower(char *str, size_t len)
{
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i) {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}

int main(){
    char *str = "THIS STRING LITERAL IS ARBITRARY";

    printf("%s\n", str);
    size_t len = strlen(str);

    char *lower = toLower(str, len);
    printf("%s", lower);
    free(lower);

    exit(EXIT_SUCCESS);
}
