/*
Exercise 2-7. Write the function rightrot(n, b) which rotates the integer n to
the right by b bit positions.
 */
 #include<stdio.h>

 unsigned rightrot(unsigned n,int b);

 int main(void)
 {
     printf("%u",(unsigned)rightrot((unsigned)8,(int)1));

     return 0;
 }

 /* rightrot: rotate x to right by n bit positions */

 unsigned rightrot(unsigned n,int b)
 {
     int wordlength(void);
     unsigned rbit;/* rightmost bit */

     rbit = n << (wordlength() - b);
     n = n >> b;
     n = n | rbit;

     return n;
 }

 int wordlength(void)
 {
     int i;
     unsigned v = (unsigned) ~0;

     for(i=1;(v=v>>1)>0;i++)
         ;
     return i;
 }
