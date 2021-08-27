/*
Exercise 7-1. Write a program which will print arbitrary input in a sensible way.
As a minimum, it should print non-graphic characters in octal or hex (according
to local custom), and fold long lines.
 */
 #include<stdio.h>
 #include<ctype.h>

 #define MAXLINE 100 /* maximum number of chars in one line */
 #define OCTLEN  6   /* length of an octal value */

 
 int main(void)
 {
     int c,pos;
     int inc(int pos,int n);

     pos = 0;        /* position in the line */

     while((c=getchar()) != EOF)
         if(iscntrl(c) || c == ' ')
         {
             /* non-graphic or blank character */
             pos = inc(pos,OCTLEN);
             printf("\\%03o",c);
             /* newline character */
             if(c == '\n')
                 pos = 0;
             putchar('\n');
         }
         else
         {
             /* graphic character */
             pos = inc(pos,1);
             putchar(c);
         }
     return 0;
 }

 /* inc : increment position counter for output */
 int inc(int pos,int n)
 {
     if(pos + n < MAXLINE)
         return pos + n;
     else
     {
         putchar('\n');
         return n;
     }
 }
