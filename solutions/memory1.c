#include <stdio.h>

/* rm a.out ; gcc memory1.c ; a.out */

int main()
{
    char line[256];
    char memory[256];
    char opcode;
    int count,address,value;
    int debug = 0;

    while(fgets(line, 256, stdin) != NULL) {
        if ( line[0] == 'X' ) break;
        if ( line[0] == '*' ) {
            printf("%s\n",line);
            continue;
        }
        if ( debug ) printf("\nLine: %s\n",line);
        count = sscanf(line, "%d %c %d", &address, &opcode, &value);
        if ( count != 3 ) continue;
        if ( debug ) printf("address: %d opcode: %c value: %d\n", address, opcode, value);

        /* Do something here */

        if ( opcode == '=' ) {
            memory[address] = value;
        } else if ( opcode == '+' ) {
            memory[address] = memory[address] + value;
        } else if ( opcode == '-' ) {
            memory[address] = memory[address] - value;
        }

        if ( debug ) printf("Memory:\n%s\n", memory);

    }

    printf("Memory:\n%s\n", memory);
}

/*
* Beginning
0 = 72
1 = 101
2 = 108
3 = 108
4 = 108
4 + 3
5 = 10
6 = 100
6 + 19
7 = 111
8 = 114
9 = 108
10 = 101
10 - 1
X 

*/

