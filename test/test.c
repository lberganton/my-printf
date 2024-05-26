#include "myprintf.h"
#include <stdio.h>

int main(void) {
  my_printf("%-12sA\n", "123456789");
  printf("%-12sA\n", "123456789");
  return 0;
}
