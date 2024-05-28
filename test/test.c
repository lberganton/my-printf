#include "myprintf.h"
#include <stdio.h>

int main(void) {
  // int rm = my_printf("%c!\n", '\0');
  // int rs = printf("%c!\n", '\0');
  // printf("RM: %d\nRS: %d\n", rm, rs);

  for (int i = 0; i < 1000; i++) {
    my_printf("%#32X %-64u %50s\n", 60, 50042, "Joia de mais");
  }
  for (int i = 0; i < 1000; i++) {
    my_printf("%032d %-4s %-500s\n", 32, "Eu gosto de sambar", "Joia de mais");
  }

  return 0;
}
