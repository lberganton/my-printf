#include "myprintf.h"
#include <stdio.h>
#include <limits.h>

int main(void) {
  int rm = my_printf("%d\n", 54);
  int rs = printf("%d\n", 54);
  printf("RM: %d\nRS: %d\n", rm, rs);
  return 0;
}
