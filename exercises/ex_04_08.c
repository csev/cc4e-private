/*
Exercise 4-8. Write a recursive version of the function reverse(s), which reverses the string s.
 */
 #include<stdio.h>
 #include<string.h>

 #define MAXLINE 100

 int mgetline(char line[],int maxline);
 void reverse(char s[]);

 int main(void)
 {
     char s[MAXLINE];

     mgetline(s,MAXLINE);

     reverse(s);

     printf("%s",s);

     return 0;
 }

 int mgetline(char s[],int lim)
 {
     int i,c;

     for(i=0;i<lim-1 && (c=getchar())!=EOF && c!='\n';++i)
         s[i] = c;

     if(c=='\n')
         s[i++]='\n';

     s[i]='\0';
 }

 void reverse(char s[])
 {
     void reverser(char s[],int i,int len);

     reverser(s,0,strlen(s));
 }

 /* reverser: reverse string s in place; recursive */

 void reverser(char s[],int i,int len)
 {
     int c,j;

     j = len - (i + 1);

     if( i < j )
     {
         c = s[i];
         s[i] = s[j];
         s[j] = c;

         reverser(s,++i,len);
     }
 }
