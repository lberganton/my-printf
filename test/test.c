/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: test.c
 * Author: Lucas Berganton
 * Created: 05/25/2024
 *
 * This file make tests comparing the output and return of GCC printf and
 * MyPrintf.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "myprintf.h"
#include <stdarg.h>
#include <stdio.h>

static int count = 1;

void test(const char *format, ...) {
  va_list args1, args2;
  va_start(args1, format);
  va_copy(args2, args1);

  printf("Teste %d\n", count++);
  int return_my = my_vprintf(format, args1);
  int return_std = vprintf(format, args2);
  printf("Retorno MY: %d\tRetorno STD: %d\n\n", return_my, return_std);

  va_end(args1);
  va_end(args2);
}

int main(void) {
  test("%c %d %f\n", 'A', -15, 53.423);
  test("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  test("%-32.3s %d %f\n", "123456789", -15, 53.423);
  test("%+06d %#05X %05.2f\n", 32, 60, 1.129);
  test("%.3f %.3f %.16f\n", 0.1289, 0.1285, 0.123);
  test("%f %f %f\n", 0.9999995, 0.9999994, 0.9999996);
  test("%+0*.*f\n", 32, 2, 15.123);

  return 0;
}
