/*
Exercise 1-14. Revise the main routine of the longest-line program so it will
correctly print the length of arbitrarily long input lines, and as much as
possible of the text.
 */
#include <stdio.h>

#define MAXLINE 1000

int mgetline(char line[],int lim);

void copy(char to[],char from[]);


int main(void)
{
    int len,max;
    char line[MAXLINE], maxline[MAXLINE];

    max =0;

    while((len = mgetline(line, MAXLINE)) > 0)
    {
        if(len > max)
        {
            max = len;
            copy(maxline,line);
        }
    }

    if(max>0)
        printf("%s",maxline);
}


int mgetline(char s[],int lim)
{
    int i,c;

    for(i=0;i<lim-1 && (c=getchar())!=EOF && c !='\n';++i)
        s[i] = c;

    if(c == '\n')
    {
    s[i] = c;
    ++i;
    }

    s[i] = '\0';
    return i;
}

void copy(char to[],char from[])
{
    int i;
    i=0;

    while((to[i]=from[i]) != '\0')
    ++i;
}
