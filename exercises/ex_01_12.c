/*
Exercise 1-12. Write a program to print a histogram of the lengths of words in
its input. It is easiest to draw the histogram horizontally; a vertical orientation
is more challenging.
 */
#include <stdio.h>

int main(void)
{
  int c;

  while((c=getchar()) != EOF)
  {
    if( c == ' ' || c == '\n' || c == '\t')
      putchar('\n');
    else
      putchar('*');
  }
  return 0;
}
