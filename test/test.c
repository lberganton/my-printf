#include "myprintf.h"
#include <stdio.h>
#include <limits.h>

int main(void) {
  my_printf("%#08b\n", 2);
  printf("%#o\n", 54);
  return 0;
}
