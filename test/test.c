#include "myprintf.h"
#include <stdio.h>
#include <limits.h>

int main(void) {
  my_printf("%#o\n", 54);
  printf("%#o\n", 54);
  return 0;
}
