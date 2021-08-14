/*
Exercise 2-9. In a 2's complement number system, x & ( x-1 ) deletes the rightmost
1-bit in x. (Why?) Use this observation to write a faster version of bitcount.
 */
 #include<stdio.h>

 int bitcount(unsigned x);

 int main(void)
 {
     printf("%d",bitcount((unsigned)12));
 }

 int bitcount(unsigned x)
 {
     int b;

     for(b=0; x!=0; x &= x-1)
         ++b;

     return b;
 }
