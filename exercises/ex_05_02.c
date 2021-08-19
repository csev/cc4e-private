/*
Exercise 5-2. Write a pointer version of the function strcat which we showed in
Chapter 2: strcat(s, t) copies the string t to the end of s.
 */

 #include<stdio.h>
 #define MAXLINE 1000

 int mgetline(char line[],int maxline);
 void mystrcat(char *,char *);

 int main(void)
 {
     int len;
     char s[MAXLINE],t[MAXLINE];

     putchar('s');
     putchar(':');
     mgetline(s,MAXLINE);

     putchar('t');
     putchar(':');
     mgetline(t,MAXLINE);

     mystrcat(s,t);

     printf("%s",s);

     return 0;
 }

 int mgetline(char s[],int lim)
 {
     int c,i;

     for(i=0;i<lim-1 && (c=getchar()) !=EOF && c!='\n';++i)
         s[i] = c;

     if(c == '\n')
     {
         s[i] = c;
         ++i;
     }

     s[i] = '\0';

     return i;
 }

 void mystrcat(char *s,char *t)
 {
     while(*s!='\0')
         s++;
     s--;             /* goes back to \0 char */
     while((*s=*t)!='\0')
     {
         s++;
         t++;
     }
 }
