/*
Exercise 4-9. Define a macro swap(x, y) which interchanges its two int arguments.
(Block structure will help.)
 */
 #include<stdio.h>

 #define swap(t,x,y) { t _z; \
              _z = x;\
               x = y;\
               y = _z; }

 int main(void)
 {
     char x,y;
     x='a';
     y='b';
     printf("x= %c \t y= %c\n",x,y);
     swap(char,x,y);
     printf("x=%c \t y=%c\n",x,y);
 }
