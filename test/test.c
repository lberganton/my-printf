#include "myprintf.h"
#include <stdio.h>

int main(void) {
  int rm = my_printf("%f\n", 3.99);
  int rs = printf("%f\n", 3.99);
  printf("RM: %d\nRS: %d\n", rm, rs);

  return 0;
}
