/*
Exercise 4-1. Write the function rindex(s, t) , which returns the position of the
rightmost occurrence of t in s, or -1 if there is none.
 */
 #include <stdio.h>
 #define MAXLINE 1000 /* maximum input line length */
 int mgetline(char line[], int max);
 int strindex(char source[], char searchfor[]);
 char pattern[] = "ould"; /* pattern to search for */
 /* find all lines matching pattern */
 int main()
 {
     char line[MAXLINE];
     int found = 0;
     while (mgetline(line, MAXLINE) > 0)
         if (rindex(line, pattern) >= 0) {
             printf("%s", line);
             found++;
         }
         return found;
 }

 /* getline: get line into s, return length */
 int mgetline(char s[], int lim)
 {
     int c, i;
     i = 0;
     while (--lim > 0 && (c=getchar()) != EOF && c != '\n')
         s[i++] = c;
     if (c == '\n')
         s[i++] = c;
     s[i] = '\0';
     return i;
 }

 /* rindex: return index of t in s, -1 if none */
 int rindex(char s[], char t[])
 {
     int i, j, k;
     for (i = 0; s[i] != '\0'; i++) {
         for (j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
             ;
         if (k > 0 && t[k] == '\0')
             return i;
     }
     return -1;
 }
