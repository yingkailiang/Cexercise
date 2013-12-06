#include <stdio.h>
#include <stdlib.h>

int main() {
  char *a = malloc(24);
  free(a);
  free(a);
  return 1;
}
