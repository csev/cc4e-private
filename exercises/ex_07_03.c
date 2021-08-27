/*
Exercise 7-3. Write a program to compare two files, printing the first line and
character position where they differ.
 */
// #include <stdio.h>
// #include <stdlib.h>
//
// /* Function declaration */
// int compareFile(FILE * fPtr1, FILE * fPtr2, int * line, int * pos);
//
//
// int main()
// {
//     /* File pointer to hold reference of input file */
//     FILE * fPtr1;
//     FILE * fPtr2;
//     char path1[100];
//     char path2[100];
//
//     int diff;
//     int line, pos;
//
//
//     /* Input path of files to compare */
//     printf("Enter path of first file: ");
//     scanf("%s", path1);
//     printf("Enter path of second file: ");
//     scanf("%s", path2);
//
//
//     /*  Open all files to compare */
//     fPtr1 = fopen(path1, "r");
//     fPtr2 = fopen(path2, "r");
//
//     /* fopen() return NULL if unable to open file in given mode. */
//     if (fPtr1 == NULL || fPtr2 == NULL)
//     {
//         /* Unable to open file hence exit */
//         printf("\nUnable to open file.\n");
//         printf("Please check whether file exists and you have read privilege.\n");
//         exit(EXIT_FAILURE);
//     }
//
//
//     /* Call function to compare file */
//     diff = compareFile(fPtr1, fPtr2, &line, &pos);
//
//     if (diff == 0)
//     {
//         printf("\nBoth files are equal.");
//     }
//     else
//     {
//         printf("\nFiles are not equal.\n");
//         printf("Line: %d, Position: %d\n", line, pos);
//     }
//
//
//     /* Finally close files to release resources */
//     fclose(fPtr1);
//     fclose(fPtr2);
//
//     return 0;
// }
//
//
// /**
//  * Function to compare two files.
//  * Returns 0 if both files are equivalent, otherwise returns
//  * -1 and sets line and col where both file differ.
//  */
// int compareFile(FILE * fPtr1, FILE * fPtr2, int * line, int * pos)
// {
//     char ch1, ch2;
//
//     *line = 1;
//     *pos  = 0;
//
//     do
//     {
//         // Input character from both files
//         ch1 = fgetc(fPtr1);
//         ch2 = fgetc(fPtr2);
//
//         // Increment line
//         if (ch1 == '\n')
//         {
//             *line += 1;
//             *pos = 0;
//         }
//
//         // If characters are not same then return -1
//         if (ch1 != ch2)
//             return -1;
//
//         *pos  += 1;
//
//     } while (ch1 != EOF && ch2 != EOF);
//
//
//     /* If both files have reached end */
//     if (ch1 == EOF && ch2 == EOF)
//         return 0;
//     else
//         return -1;
// }


#include<stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp1, *fp2;
    void filecmp(FILE *, FILE *);

    if (argc == 1)
    {
        /* no args; copy standard input */
        printf("Please provide two files");
        return 1;
    }
    else
        while (--argc > 0) {
            if ((fp1 = fopen(argv[1], "r")) == NULL) {
                printf("cat: can't open %s\n", *argv);
                return 1;
            }

            if ((fp2 = fopen(argv[2], "r")) == NULL) {
                printf("cat: can't open %s\n", *argv);
                return 1;
            }
            filecmp(fp1, fp2);
            fclose(fp1);
            fclose(fp2);
        }

    return 0;
}


void filecmp(FILE *fp1, FILE *fp2)
{
    int f1, f2;
    while (1) {
        if ((f1 = getc(fp1)) == EOF)
            break;
        if ((f2 = getc(fp2)) == EOF)
            break;

        /* TODO: Senthil Print the full sentence */

        if (f1 != f2) {
            putchar(f1);
            putchar(f2);
            break;
        }
    }
}
