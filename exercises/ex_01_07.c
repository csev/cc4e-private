/*
Exercise 1-7. Write a program to copy its input to its output, replacing each string of one or more blanks by a single blank.
 */
 #include <stdio.h>
 int main(int arg, char *argv[]){
 char c = 0;
 long blank = 0;
 long tab   = 0;
 while((c=getchar())!= EOF){
  if(c == ' '){
     ++blank;
  }

  if(c != ' '){
      if(blank>=1){
        printf("%c", ' ');
        blank = 0;
        printf("%c", c);
         }
  else{
         printf("%c", c);
      }
   }

  } 
 return 0;
 }
