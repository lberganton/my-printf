#include "myprintf.h"
#include <stdio.h>

int main(void) {
  int rm = my_printf("%f\n", 2.9999994);
  int rs = printf("%f\n", 2.9999994);
  printf("RM: %d\nRS: %d\n", rm, rs);

  return 0;
}
