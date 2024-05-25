/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.c
 * Author: Lucas Berganton
 * Created: 05/25/2024
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "myprintf.h"
#include <stdio.h>

#define FLAG_LEFT 0x1
#define FLAG_SIG 0x2
#define FLAG_SPACE 0x4
#define FLAG_ZERO 0x8

static inline int char_out(FILE *file, int ch) {
  fwrite(&ch, sizeof(int), 1, file);
}

#define INT_NEGATIVE 0x1
#define INT_SIGNED 0x2
#define INT_HEX 0x4
#define INT_OCT 0x8

static int print_integer(FILE *file, long unsigned value, int flags) {
  static const char nums[] = "0123456789ABCDEF";

  int base = INT_HEX ? 16 : INT_OCT ? 8 : 10;
  int count = 0;

  char stack[16];
  int top = -1;

  if (flags & INT_NEGATIVE) {
    char_out(file, '-');
    count++;
  } else if (flags & INT_SIGNED) {
    char_out(file, '+');
    count++;
  }

  while (value) {
    stack[++top] = nums[value % base];
    if (flags & INT_HEX && stack[top] > 9)
      stack[top] += 20;
    value /= base;
  }

  while (top >= 0) {
    char_out(file, stack[top--]);
    count++;
  }

  return count;
}

int my_vfprintf(FILE *file, const char *format, va_list args) {
  int flags = 0;
  int count = 0;

  while (*format) {
    if (*format != '%') {
      char_out(file, *format++);
      count++;
      continue;
    }

    format++;

    if (*format == '-') {
      flags |= FLAG_LEFT;
      format++;
    }
    if (*format == '+') {
      flags |= FLAG_SIG;
      format++;
    }
    
    
  }
}
