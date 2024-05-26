/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.c
 * Author: Lucas Berganton
 * Created: 05/25/2024
 * 
 * A simple printf implementation.
 * Ins't compatible with all features of the standard printf.
 * 
 * Here is a list with all allowed format specifiers:
 *    %d: For decimal numbers;
 *    %i: For decimal numbers;
 *    %u: For unsigned decimal number;
 *    %x: For hexadecimal numbers (lowercase);
 *    %X: For hexadecimal numbers (upercase);
 *    %o: for octal numbers;
 *    %f: For float numbers;
 *    %c: For ascii characters;
 *    %s: For ascii strings.
 * 
 * Here is a list with all allowed flags:
 *    Left widthed: '-';
 *    Force signal: '+';
 *    Space padding: ' ';
 *    Zero paddding: '0';
 *    Width: A integer;
 *    Precision: '.' and a integer;
 *    Flag passed as a argument: '*'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "myprintf.h"
#include <stdio.h>

#define FLAG_LEFT 0x1
#define FLAG_SIG 0x2
#define FLAG_PLUS 0x4
#define FLAG_SPACE 0x8
#define FLAG_ZERO 0x10

static inline void char_out(FILE *file, int ch) {
  fwrite(&ch, sizeof(int), 1, file);
}

static inline _Bool is_digit(const char ch) { return ch >= '0' && ch <= '9'; }

int my_vfprintf(FILE *file, const char *format, va_list args) {
  int flags = 0;
  int width = 0;
  int precision = 0;
  int count = 0;

  while (*format) {
    // If the current char isn't a format specifier, print it and go the next
    // one.
    if (*format != '%') {
      char_out(file, *format++);
      count++;
      continue;
    }

    format++;

    // Get the format flags.
    while (1) {
      if (*format == '-')
        flags |= FLAG_LEFT;
      else if (*format == '0')
        flags |= FLAG_ZERO;
      else if (*format == '+')
        flags |= FLAG_PLUS;
      else if (*format == ' ')
        flags |= FLAG_SPACE;
      else
        break;
      
      format++;
    }
    

    // Get the width.
    if (*format == '*') {
      width = va_arg(args, int);
      if (flags < 0) {
        flags |= FLAG_LEFT;
        width = -width;
      }
      format++;
    } else {
      while (is_digit(*format)) {
        width = (width * 10) + (*format - '0');
        format++;
      }
    }

    // Get the precision.
    if (*format == '.') {
      format++;

      if (*format == '*') {
        precision = va_arg(args, int);

        if (precision < 0)
          precision = MY_PRINTF_DEFAULT_PRECISION;

        format++;
      } else {
        while (is_digit(*format)) {
          precision = (precision * 10) + (*format - '0');
          format++;
        }

        if (precision > MY_PRINTF_MAX_PRECISION) {
          precision = MY_PRINTF_MAX_PRECISION;
        }
      }
    }
  }

  return count;
}

int my_fprintf(FILE *file, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int count = my_vfprintf(file, format, args);
  va_end(args);
  return count;
}

int my_vprintf(const char *format, va_list args) {
  return my_vfprintf(stdout, format, args);
}

int my_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  int count = my_vfprintf(stdout, format, args);
  va_end(args);
  return count;
}
