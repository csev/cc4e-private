/*
Exercise 1-20. Write the program entab which replaces strings of blanks by the
minimum number of tabs and blanks to achieve the same spacing. Use the same tab
stops as for detab.
 */
 #include <stdbool.h>
 #include <stdio.h>

#define TABSTOP 4

 int main(void) {

     char c;
     int  pos;
     int  numtabs   = 0;
     int  numspaces = 0;

     for (pos = 1; (c = getchar()) != EOF; pos++) {
         if (c == ' ') {
             if (pos % TABSTOP != 0)
                 numspaces++;
             else {
                 numspaces = 0;
                 numtabs++;
             }
         } else {
             for ( ; numtabs > 0; numtabs--)
                 (void)putchar('\t');
             if (c == '\t')
                 numspaces = 0;
             else
                 for ( ; numspaces > 0; numspaces--)
                     (void)putchar(' ');
             (void)putchar(c);
             if (c == '\n')
                 pos = 0;
             else if (c == '\t')
                 pos = pos + (TABSTOP - (pos - 1) % TABSTOP) - 1;
         }
     }

     return 0;
 }
