#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int heap_fill(char *str)
{
    //test string, used to determine if the contents of the target buffer have changed
    char *test_string = "These are the original contents of the buffer on the heap";
    
    //buffer1 is the buffer we are going to overflow, buffer2 is our target
    char *buffer1, *buffer2;
    buffer1 = (char *) malloc(12 * sizeof(char));
    buffer2 = (char *) malloc(sizeof(test_string)); 

    //copy the test string into the second buffer, and display its contents
    strcpy(buffer2, test_string);
    printf("%s\n", test_string);

    //overflow the 1st buffer into the second
    strcpy(buffer1, str);

    //print the contents of the second buffer, they should be altered
    printf("%s\n", buffer2);

    //try to free the buffer2, glibc should detect that the heap segment
    //headers have been modified/corrupted
    free(buffer2);

    return 1;
}

int main(int argc, char **argv)
{
    char str[100];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 100, badfile);
    heap_fill(str);

    printf("Returned Properly\n");
}
