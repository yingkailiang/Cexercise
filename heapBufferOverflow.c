#include <stdio.h>
#include <stdlib.h>

int main()
{
 int *a =(int*) malloc(sizeof(int));
 *a=3;
 free(a);

 return 1;
}
