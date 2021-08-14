/*
Exercise 3-5. Write a version of itoa which accepts three arguments instead of two.
The third argument is a minimum field width; the converted number must be padded
with blanks on the left if necessary to make it wide enough.
 */
 #include<stdio.h>
#include<string.h>

#define MAXLIMIT 100

void itoa(int n,char s[],int w);
void reverse(char s[]);

int main(void)
{
    int number,width;
    char str[MAXLIMIT];

    number= -343565;
    width=10;

    itoa(number,str,width);

    printf("%s",str);

    return 0;
}

void itoa(int n,char s[],int w)
{
    int i,sign;

    if((sign=n) < 0)
        n = -n;
    i=0;

    do
    {
        s[i++] = (n %10) + '0';

    }while((n/=10)>0);

    if(sign <0)
        s[i++]='-';

    while(i<w)
        s[i++]=' ';

    s[i]='\0';

    reverse(s);
}

void reverse(char s[])
{
    int i,j,c;

    for(i=0,j=strlen(s)-1;i<j;i++,j--)
        c=s[i],s[i]=s[j],s[j]=c;
}
