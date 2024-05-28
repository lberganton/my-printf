#include "myprintf.h"
#include <stdio.h>

int main(void) {
  int rm = my_printf("%;\n", 5);
  int rs = printf("%.f\n", 5);
  printf("RM: %d\nRS: %d\n", rm, rs);

  return 0;
}
