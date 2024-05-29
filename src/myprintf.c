/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.c
 * Author: Lucas Berganton
 * Created: 05/25/2024
 *
 * A simple printf implementation.
 * It does NOT support all features of standard printf.
 *
 * To format specifier its tipically set as:
 *    %[flags][width][.precision][length]type
 *
 * Here is a list with all allowed format specifiers:
 *    %i: For decimal numbers;
 *    %d: For decimal numbers;
 *    %u: For unsigned decimal number;
 *    %x: For hexadecimal numbers (lowercase);
 *    %X: For hexadecimal numbers (uppercase);
 *    %o: For octal numbers;
 *    %b: For binary numbers (lowercase);
 *    %B: For binary numbers (uppercase);
 *    %f: For float numbers (lowercase);
 *    %F: For float numbers (uppercase);
 *    %c: For ascii characters;
 *    %s: For ascii strings;
 *    %%: For '%' character.
 *
 * Here is a list with all allowed flags:
 *    Left-align: '-';
 *    Force signal: '+';
 *    Space padding: ' ';
 *    Zero paddding: '0';
 *    Alternate form: '#';
 *    Width: A integer;
 *    Precision: '.' and a integer;
 *    Width or precision passed as a argument: '*'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "myprintf.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>

#define FLAG_LEFT 0x1
#define FLAG_SIG 0x2
#define FLAG_SPACE 0x4
#define FLAG_ZERO 0x8
#define FLAG_HASH 0x10
#define FLAG_LONG 0x20
#define FLAG_LONG_LONG 0x40
#define FLAG_SHORT 0x80
#define FLAG_SHORT_SHORT 0x100
#define FLAG_HEX 0x200
#define FLAG_OCT 0x400
#define FLAG_BIN 0x800
#define FLAG_UPPER 0x1000
#define FLAG_PREC 0x2000

static inline void _char_out(FILE *file, int ch) {
  fwrite(&ch, sizeof(int), 1, file);
}

static int _pad_out(FILE *file, size_t length, int ch) {
  int count = 0;
  for (size_t i = 0; i < length; i++) {
    _char_out(file, ch);
    count++;
  }
  return count;
}

static int _str_out(FILE *file, const char *str, size_t length) {
  int count = 0;
  for (size_t i = 0; i < length && *str; i++) {
    _char_out(file, *str++);
    count++;
  }
  return count;
}

static int _strrev_out(FILE *file, const char *str, size_t length) {
  str += length - 1;
  for (size_t i = 0; i < length; i++) {
    _char_out(file, *str--);
  }
  return length;
}

static inline bool _is_digit(const char ch) {
  return (ch >= '0') && (ch <= '9');
}

static int _itoa_out(FILE *file, int flags, int width, bool negative,
                     long long unsigned value) {
  int count;
  char stack[32];
  int top = 0;

  int base = flags & FLAG_HEX   ? 16
             : flags & FLAG_OCT ? 8
             : flags & FLAG_BIN ? 2
                                : 10;

  while (value) {
    // Get the last digit of value.
    char digit = value % base;

    // Test if its a hexadecimal digit.
    if (digit >= 10) {
      digit = (digit % 10 + 'a') - (flags & FLAG_UPPER ? 32 : 0);
    } else {
      digit = digit + '0';
    }

    // Put into the stack.
    stack[top++] = digit;

    // Remove the last digit from value.
    value /= base;
  }

  // Zero padding.
  if (flags & FLAG_ZERO) {
    int lenght = width - top;

    if (negative || flags & FLAG_SIG || flags & FLAG_SPACE) {
      lenght--;
    }

    if (flags & FLAG_HASH) {
      if ((flags & FLAG_HEX) || (flags & FLAG_BIN)) {
        lenght--;
      }
      lenght--;
    }

    while (lenght > 0) {
      stack[top++] = '0';
      lenght--;
    }
  }

  // Put the signal if necessary.
  if (negative) {
    stack[top++] = '-';
  } else if (flags & FLAG_SIG) {
    stack[top++] = '+';
  } else if (flags & FLAG_SPACE) {
    stack[top++] = ' ';
  }

  // Put the alternate form if necessary.
  if (flags & FLAG_HASH) {
    if (flags & FLAG_HEX) {
      stack[top++] = flags & FLAG_UPPER ? 'X' : 'x';
    } else if (flags & FLAG_BIN) {
      stack[top++] = flags & FLAG_UPPER ? 'B' : 'b';
    }

    stack[top++] = '0';
  }

  // No padding if the whole number representation its greater than the width.
  if (top > width) {
    return _strrev_out(file, stack, top);
  }

  // Print the number first in case of left padding and then pad the rest. In
  // the other hand, do the opposite.
  if (flags & FLAG_LEFT) {
    count = _strrev_out(file, stack, top);
    count += _pad_out(file, width - top, ' ');
  } else {
    count = _pad_out(file, width - top, ' ');
    count += _strrev_out(file, stack, top);
  }

  return count;
}

static int _ftoa_out(FILE *file, int flags, int width, int precision,
                     bool negative, double value) {
  int count = 0;
  char stack[MY_PRINTF_MAX_PRECISION + 33];
  int top = 0;

  // Special values
  if (value != value) {
    return _str_out(file, flags & FLAG_UPPER ? "NAN" : "nan", 3);
  }
  if (value > DBL_MAX) {
    if (flags & FLAG_UPPER) {
      return _str_out(file, flags & FLAG_SIG ? "+INF" : "INF", 4);
    }
    return _str_out(file, flags & FLAG_SIG ? "+inf" : "inf", 4);
  }
  if (value < -DBL_MAX) {
    if (flags & FLAG_UPPER) {
      return _str_out(file, flags & FLAG_SIG ? "-INF" : "INF", 4);
    }
    return _str_out(file, flags & FLAG_SIG ? "-inf" : "inf", 4);
  }

  if (negative) {
    value = -value;
  }

  double multiplier = 1.0;

  for (int i = 0; i < precision; i++) {
    multiplier *= 10.0;
  }

  // Round the number.
  value = (long long unsigned)(value * multiplier + 0.5);

  long long unsigned int_part = (long long unsigned)value / multiplier;
  long long unsigned frac_part =
      (long long unsigned)value - int_part * multiplier;

  while (frac_part) {
    // Get the last digit of value.
    char digit = frac_part % 10 + '0';

    // Put into the stack.
    stack[top++] = digit;

    // Remove the last digit from value.
    frac_part /= 10;
  }

  // Fill with zeros if necessary.
  while (top < precision) {
    stack[top++] = '0';
  }

  // Put a dot if necessary.
  if (precision) {
    stack[top++] = '.';
  }

  // Same algorithm of the fractional part for the integer part.
  if (int_part) {
    do {
      char digit = int_part % 10 + '0';
      stack[top++] = digit;
      int_part /= 10;
    } while (int_part);
  } else {
    stack[top++] = '0';
  }

  // Zero padding.
  if (flags & FLAG_ZERO) {
    int lenght = width - top;

    if (negative || flags & FLAG_SIG || flags & FLAG_SPACE) {
      lenght--;
    }

    while (lenght > 0) {
      stack[top++] = '0';
      lenght--;
    }
  }

  // Put the signal if necessary.
  if (negative) {
    stack[top++] = '-';
  } else if (flags & FLAG_SIG) {
    stack[top++] = '+';
  } else if (flags & FLAG_SPACE) {
    stack[top++] = ' ';
  }

  // No padding if the whole number representation its greater than the width.
  if (top > width) {
    return _strrev_out(file, stack, top);
  }

  // Print the number first in case of left padding and then pad the rest. In
  // the other hand, do the opposite.
  if (flags & FLAG_LEFT) {
    count = _strrev_out(file, stack, top);
    count += _pad_out(file, width - top, ' ');
  } else {
    count = _pad_out(file, width - top, ' ');
    count += _strrev_out(file, stack, top);
  }

  return count;
}

int my_vfprintf(FILE *file, const char *format, va_list args) {
  int flags = 0;
  int count = 0;
  int width = 0;
  int precision = 0;

  while (*format) {
    // If the current char isn't a format specifier, print it and go the next
    // one.
    if (*format != '%') {
      _char_out(file, *format++);
      count++;
      continue;
    }

    format++;

    // Get the flags.
    while (true) {
      if (*format == '-') {
        flags |= FLAG_LEFT;
      } else if (*format == '0') {
        flags |= FLAG_ZERO;
      } else if (*format == '+') {
        flags |= FLAG_SIG;
      } else if (*format == ' ') {
        flags |= FLAG_SPACE;
      } else if (*format == '#') {
        flags |= FLAG_HASH;
      } else {
        break;
      }

      format++;
    }

    // Disable the zero-padding flag if a left-padding flag was provided.
    if (flags & FLAG_LEFT) {
      flags &= ~FLAG_ZERO;
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
      while (_is_digit(*format)) {
        width = (width * 10) + (*format - '0');
        format++;
      }
    }

    // Get the precision.
    if (*format == '.') {
      format++;

      flags |= FLAG_PREC;
      precision = 0;

      if (*format == '*') {
        precision = va_arg(args, int);

        if (precision < 0) {
          precision = 0;
          precision = MY_PRINTF_DEFAULT_PRECISION;
        }

        format++;
      } else {
        if (_is_digit(*format)) {
          do {
            precision = (precision * 10) + (*format - '0');
            format++;
          } while (_is_digit(*format));
        }

        if (precision > MY_PRINTF_MAX_PRECISION) {
          precision = MY_PRINTF_MAX_PRECISION;
        }
      }
    }

    // Get the length.
    if (*format == 'l') {
      flags |= FLAG_LONG;
      format++;
      if (*format == 'l') {
        flags |= FLAG_LONG_LONG;
        format++;
      }
    } else if (*format == 'h') {
      flags |= FLAG_SHORT;
      format++;
      if (*format == 'h') {
        flags |= FLAG_SHORT_SHORT;
        format++;
      }
    }

    // The main format specifier.
    switch (*format) {
    case 'i':
    case 'd': {
      bool negative = false;
      long long value;

      if (flags & FLAG_LONG_LONG) {
        value = va_arg(args, long long);
      } else if (flags & FLAG_LONG) {
        value = va_arg(args, long);
      } else if (flags & FLAG_SHORT_SHORT) {
        value = (char)va_arg(args, long);
      } else if (flags & FLAG_SHORT) {
        value = (short)va_arg(args, int);
      } else {
        value = va_arg(args, int);
      }

      if (value < 0) {
        negative = true;
        value = -value;
      }

      flags &= ~FLAG_HASH;

      count +=
          _itoa_out(file, flags, width, negative, (long long unsigned)value);
    } break;

    case 'u':
    case 'x':
    case 'X':
    case 'o':
    case 'b':
    case 'B': {
      long long unsigned value;

      if (flags & FLAG_LONG_LONG) {
        value = va_arg(args, long long unsigned);
      } else if (flags & FLAG_LONG) {
        value = va_arg(args, long);
      } else if (flags & FLAG_SHORT_SHORT) {
        value = (unsigned char)va_arg(args, unsigned);
      } else if (flags & FLAG_SHORT) {
        value = (short unsigned)va_arg(args, unsigned);
      } else {
        value = va_arg(args, unsigned int);
      }

      if (*format == 'x' || *format == 'X') {
        flags |= FLAG_HEX;
      } else if (*format == 'b' || *format == 'B') {
        flags |= FLAG_BIN;
      } else if (*format == 'o') {
        flags |= FLAG_OCT;
      }

      if (*format == 'X' || *format == 'B') {
        flags |= FLAG_UPPER;
      }

      flags &= ~(FLAG_SIG | FLAG_SPACE);

      count += _itoa_out(file, flags, width, false, value);
    } break;

    case 'f':
    case 'F': {
      if (*format == 'F') {
        flags |= FLAG_UPPER;
      }

      if (!(flags & FLAG_PREC)) {
        precision = MY_PRINTF_DEFAULT_PRECISION;
      }

      double value = va_arg(args, double);

      count += _ftoa_out(file, flags, width, precision,
                         value < 0.0 ? true : false, value);
    } break;

    case 'c': {
      count++;

      if (flags & FLAG_LEFT) {
        _char_out(file, va_arg(args, int));
        count += _pad_out(file, width ? width - 1 : 0, ' ');
      } else {
        count += _pad_out(file, width ? width - 1 : 0, ' ');
        _char_out(file, va_arg(args, int));
      }
    } break;

    case 's': {
      const char *ptr = va_arg(args, char *);
      int length = 0;

      while (ptr[length] && (flags & FLAG_PREC ? length < precision : true)) {
        length++;
      }

      if (length > width) {
        count += _str_out(file, ptr, length);
      } else if (flags & FLAG_LEFT) {
        count += _str_out(file, ptr, length);
        count += _pad_out(file, width - length, ' ');
      } else {
        count += _pad_out(file, width - length, ' ');
        count += _str_out(file, ptr, length);
      }
    } break;

    case '%':
      _char_out(file, *format);
      count++;
      break;

    default:
      _char_out(file, *format);
      count++;
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

#undef FLAG_LEFT
#undef FLAG_SIG
#undef FLAG_SPACE
#undef FLAG_ZERO
#undef FLAG_HASH
#undef FLAG_LONG
#undef FLAG_LONG_LONG
#undef FLAG_SHORT
#undef FLAG_SHORT_SHORT
#undef FLAG_HEX
#undef FLAG_OCT
#undef FLAG_BIN
#undef FLAG_UPPER
#undef FLAG_PREC
