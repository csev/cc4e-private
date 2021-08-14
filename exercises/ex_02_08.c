/*
Exercise 2-8. Write the function invert(x, p, n) which inverts (i.e., changes 1
into 0 and vice versa) the n bits of x that begin at position p, leaving the others unchanged.
 */
 #include<stdio.h>

 unsigned invert(unsigned x,int p,int n);

 int main(void)
 {
     printf("%u", (unsigned) invert((unsigned) 8, (int) 3, (int) 3));
 }

 unsigned invert(unsigned x, int p, int n)
 {
     return   x ^ (~(~0 << n) << (p + 1 - n));
 }
