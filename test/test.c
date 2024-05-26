#include "myprintf.h"
#include <stdio.h>

int main(void) {
  int rm = my_printf("%X\n", 60);
  int rs = printf("%X\n", 60);

  printf("RM: %d\nRS: %d\n", rm, rs);
  
  return 0;
}
