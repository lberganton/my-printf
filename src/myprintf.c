/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.c
 * Author: Lucas Berganton
 * Created: 05/25/2024
 *
 * A simple printf implementation.
 * It does NOT support all features of standard printf.
 *
 * Here is a list with all allowed format specifiers:
 *    %i: For decimal numbers;
 *    %d: For decimal numbers;
 *    %u: For unsigned decimal number;
 *    %x: For hexadecimal numbers (lowercase);
 *    %X: For hexadecimal numbers (upercase);
 *    %o: for octal numbers;
 *    %b: for binary numbers (lowercase);
 *    %B: for binary numbers (upercase);
 *    %f: For float numbers;
 *    %c: For ascii characters;
 *    %s: For ascii strings.
 *
 * Here is a list with all allowed flags:
 *    Left-align: '-';
 *    Force signal: '+';
 *    Space padding: ' ';
 *    Zero paddding: '0';
 *    Width: A integer;
 *    Precision: '.' and a integer;
 *    Flag passed as a argument: '*'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "myprintf.h"
#include <stdbool.h>
#include <stdio.h>

#define FLAG_LEFT 0x1
#define FLAG_SIG 0x2
#define FLAG_SPACE 0x4
#define FLAG_ZERO 0x8
#define FLAG_HASH 0x10
#define FLAG_LONG 0x20
#define FLAG_SHORT 0x40
#define FLAG_HEX 0x80
#define FLAG_OCT 0x100
#define FLAG_BIN 0x200
#define FLAG_UPPER 0x400

static inline void char_out(FILE *file, int ch) {
  fwrite(&ch, sizeof(int), 1, file);
}

static inline bool is_digit(const char ch) { return ch >= '0' && ch <= '9'; }

static size_t strrev(FILE *file, const char *str, size_t length) {
  str += length - 1;
  for (size_t i = 0; i < length; i++)
    char_out(file, *str--);
  return length;
}

static int itoa_print(FILE *file, int flags, int width, bool negative,
                      long unsigned value) {
  int count = 0;
  char stack[32];

  int base = flags & FLAG_HEX   ? 16
             : flags & FLAG_OCT ? 8
             : flags & FLAG_BIN ? 2
                                : 10;

  while (value) {
    // Get the last digit of value.
    int digit = value % base;

    // If its a hexadecimal digit.
    if (digit > 9) {
      digit = digit % 10 + 'a';
      if (flags & FLAG_UPPER)
        digit -= 32;
    } else {
      digit = digit + '0';
    }

    stack[count++] = digit;
    value /= base;
  }

  if (flags & FLAG_ZERO) {
    int lenght = width - count;

    if (negative || flags & FLAG_SIG || flags & FLAG_SPACE)
      lenght--;

    if (flags & FLAG_HASH) {
      if (flags & FLAG_HEX)
        lenght--;
      lenght--;
    }

    while (lenght > 0) {
      stack[count++] = '0';
      lenght--;
    }
  }

  if (negative)
    stack[count++] = '-';
  else if (flags & FLAG_SIG)
    stack[count++] = '+';
  else if (flags & FLAG_SPACE)
    stack[count++] = ' ';

  if (flags & FLAG_HASH) {
    if (flags & FLAG_HEX)
      stack[count++] = flags & FLAG_UPPER ? 'X' : 'x';
    else if (flags & FLAG_BIN)
      stack[count++] = flags & FLAG_UPPER ? 'B' : 'b';
    stack[count++] = '0';
  }

  if (count > width) {
    strrev(file, stack, count);
    return count;
  }

  if (flags & FLAG_LEFT) {
    strrev(file, stack, count);

    for (int i = count; i < width; i++) {
      char_out(file, ' ');
      count++;
    }

    return count;
  }

  for (int i = count; i < width; i++) {
    char_out(file, ' ');
    count++;
  }

  strrev(file, stack, count);

  return count;
}

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
    while (true) {
      if (*format == '-')
        flags |= FLAG_LEFT;
      else if (*format == '0')
        flags |= FLAG_ZERO;
      else if (*format == '+')
        flags |= FLAG_SIG;
      else if (*format == ' ')
        flags |= FLAG_SPACE;
      else if (*format == '#')
        flags |= FLAG_HASH;
      else
        break;

      format++;
    }

    // Get the width.
    if (*format == '*') {
      width = va_arg(args, int);
      if (width < 0) {
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

    // Check the modifier.
    if (*format == 'l') {
      flags |= FLAG_LONG;
      format++;
    } else if (*format == 'h') {
      flags |= FLAG_SHORT;
      format++;
    }

    // The main format specifier.
    switch (*format) {
    case 'c': {
      if (flags & FLAG_LEFT) {
        char_out(file, va_arg(args, int));

        for (int i = 1; i < width; i++) {
          char_out(file, ' ');
          count++;
        }
      } else {
        for (int i = 1; i < width; i++) {
          char_out(file, ' ');
          count++;
        }

        char_out(file, va_arg(args, int));
      }
    } break;
    case 's': {
      const char *ptr = va_arg(args, char *);
      int length = 0;

      while (ptr[length])
        length++;

      if (length > width) {
        while (*ptr) {
          char_out(file, *ptr++);
          count++;
        }
      }

      if (flags & FLAG_LEFT) {
        while (*ptr) {
          char_out(file, *ptr++);
          count++;
        }

        for (int i = length; i < width; i++) {
          char_out(file, ' ');
          count++;
        }
      } else {
        for (int i = length; i < width; i++) {
          char_out(file, ' ');
          count++;
        }

        while (*ptr) {
          char_out(file, *ptr++);
          count++;
        }
      }
    } break;
    case 'i':
    case 'd': {
      bool negative = false;
      long value;

      if (flags & FLAG_LONG)
        value = va_arg(args, long);
      else
        value = va_arg(args, int);

      if (value < 0) {
        negative = true;
        value = -value;
      }

      count += itoa_print(file, flags, width, negative, (long unsigned)value);
    } break;
    case 'u':
    case 'x':
    case 'X':
    case 'o':
    case 'b':
    case 'B': {
      long unsigned value;

      if (flags & FLAG_LONG)
        value = va_arg(args, long unsigned);
      else
        value = va_arg(args, unsigned int);

      if (*format == 'x' || *format == 'X')
        flags |= FLAG_HEX;
      else if (*format == 'b' || *format == 'B')
        flags |= FLAG_BIN;
      else if (*format == 'o')
        flags |= FLAG_OCT;

      if (*format == 'X' || *format == 'B')
        flags |= FLAG_UPPER;

      if (flags & FLAG_SIG)
        flags &= ~FLAG_SIG;

      count += itoa_print(file, flags, width, false, value);
    } break;
    }

    flags = 0;
    width = 0;
    precision = 0;

    format++;
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
