#include "myprintf.h"
#include <stdio.h>

int main(void) {
  int rm = my_printf("%.50f\n", 3.5423);
  int rs = printf("%.50f\n", 3.5423);
  printf("RM: %d\nRS: %d\n", rm, rs);

  return 0;
}
