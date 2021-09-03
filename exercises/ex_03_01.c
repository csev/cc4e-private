/*
Exercise 3-1. Write a function expand(s, t) which converts characters like newline
and tab into visible escape sequences like \n and \t as it copies the string s to t.
Use a switch.
 */
 #include<stdio.h>
 #define MAXLINE 1000
 int mgetline(char line[],int maxline);
 void escape(char s[],char t[]);


 int main(void)
 {
     char s[MAXLINE],t[MAXLINE];

     mgetline(t,MAXLINE);

     escape(s,t);

     printf("%s",s);

     return 0;
 }

 void escape(char s[],char t[])
 {
     int i,j;

     i=j=0;

     while(t[i] != '\0')
     {
         switch(t[i])
         {
             case '\t':
                     s[j]='\\';
                     ++j;
                     s[j]='t';
                     break;
             case '\n':
                     s[j]='\\';
                     ++j;
                     s[j]='n';
                     break;
             default:
                     s[j]=t[i];
                     break;
         }
         ++i;
         ++j;
     }

     s[j]='\0';
 }

 int mgetline(char s[],int lim)
 {
     int i,c;

     for(i=0;i<lim-1 && (c=getchar())!=EOF;++i)
         s[i]=c;

     s[i]='\0';
 }
