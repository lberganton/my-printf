#include "myprintf.h"
#include <stdio.h>

int main(void) {
  // int ret_my;
  // int ret_std;

  // ret_my = my_printf("%c %d %f\n", 'A', -15, 53.423);
  // ret_std = printf("%c %d %f\n", 'A', -15, 53.423);
  // printf("Retorno MY: %d\tRetorno STD: %d\n", ret_my, ret_std);

  // ret_my = my_printf("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  // ret_std = printf("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  // printf("Retorno MY: %d\tRetorno STD: %d\n", ret_my, ret_std);

  // Adicionar precisão para %s
  my_printf("%.10s\n", "123456789");
  printf("%.10s\n", "123456789");

  return 0;
}
