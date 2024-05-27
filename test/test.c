#include "myprintf.h"
#include <stdio.h>
#include <float.h>
#include <math.h>

int main(void) {
  int rm = my_printf("%-07d\n", 60);
  int rs = printf("%-07d\n", 60);
  printf("RM: %d\nRS: %d\n", rm, rs);

  return 0;
}
