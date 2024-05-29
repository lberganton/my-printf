#include "myprintf.h"
#include <stdio.h>
#include <string.h>

int main(void) {
  int return_my;
  int return_std;

  return_my = my_printf("%c %d %f\n", 'A', -15, 53.423);
  return_std = printf("%c %d %f\n", 'A', -15, 53.423);
  printf("Retorno MY: %d\tRetorno STD: %d\n", return_my, return_std);

  return_my = my_printf("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  return_std = printf("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  printf("Retorno MY: %d\tRetorno STD: %d\n", return_my, return_std);

  return_my = my_printf("%-32.3s %d %f\n", "123456789", -15, 53.423);
  return_std = printf("%c %d %f\n", 'A', -15, 53.423);
  printf("Retorno MY: %d\tRetorno STD: %d\n", return_my, return_std);

  return_my = my_printf("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  return_std = printf("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  printf("Retorno MY: %d\tRetorno STD: %d\n", return_my, return_std);

  return_my = my_printf("%.3f %.3f %.16f\n", 0.1289, 0.1285, 0.123);
  return_std = printf("%.3f %.3f %.16f\n", 0.1289, 0.1285, 0.123);
  printf("Retorno MY: %d\tRetorno STD: %d\n", return_my, return_std);

  return 0;
}
