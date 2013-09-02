#include <stdio.h>
#include <stdbool.h>

bool comp(char *str1,char *str2);

void main()
{
 char str[10] ="hello!";
 if(comp(str,"hello!"))
 {
  printf("equal\n");
 }
 else
 {
  printf("not equal\n");
 }
}//end main

//compare two string content equal
bool comp(char *str1,char *str2)
{
 return strcmp(str1,str2)==0 ;
}//end func comp
