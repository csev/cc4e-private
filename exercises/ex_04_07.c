/*
Exercise 4-7. Adapt the ideas of printd to write a recursive version of itoa;
that is, convert an integer into a string with a recursive routine.
 */
 #include<stdio.h>
 #include<math.h>

 #define MAXLEN 100

 void itoa(int n,char s[]);


 int main(void)
 {
     int n;
     char s[MAXLEN];

     n = 424242;

     itoa(n,s);

     printf("%s",s);

     return 0;
 }

 void itoa(int n,char s[])
 {
     static int i;

     if(n/10)
         itoa(n/10,s);
     else
     {
         i = 0;
         if( n < 0)
             s[i++]='-';
     }

     s[i++] = abs(n) % 10 + '0';

     s[i] = '\0';

 }

/*
 Explanation
 The main part of this program is the itoa routine, which takes an integer n and string s and is called recursively.
 */
