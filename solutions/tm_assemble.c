#include <stdio.h>

int main()
{
    char memory[256], token[256];
    int position = 0, value;

    while(scanf("%s",token) == 1 ) {
        if ( token[0] == '>' ) position++;
        else if ( token[0] == '<' ) position--;
        else if ( sscanf(token, "%d", &value) == 1 ) memory[position] = value;
    }
    printf("Memory:\n%s\n", memory);
}


