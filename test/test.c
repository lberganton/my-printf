#include "myprintf.h"
#include <stdio.h>
#include <float.h>
#include <math.h>

int main(void) {
  int rm = my_printf("%32d!\n", 32);
  int rs = printf("%32d!\n", 32);
  printf("RM: %d\nRS: %d\n", rm, rs);

  return 0;
}
