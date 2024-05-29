/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.h
 * Author: Lucas Berganton
 * Created: 05/25/2024
 *
 * This header provides the declaration of all implemented printf's, in addition
 * to two macros that defines the behavior of the float numbers format.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma once

#include <stdarg.h>
#include <stdio.h>

// Default floating-point precision.
// Default: 6
#ifndef MY_PRINTF_DEFAULT_PRECISION
#define MY_PRINTF_DEFAULT_PRECISION 6
#endif

// Max floating-point precision. If is gave a precision greater than this, the
// number will be truncated.
// Default: 16
#ifndef MY_PRINTF_MAX_PRECISION
#define MY_PRINTF_MAX_PRECISION 16
#endif

int my_vfprintf(FILE *file, const char *format, va_list args);

int my_fprintf(FILE *file, const char *format, ...);

int my_vprintf(const char *format, va_list args);

int my_printf(const char *format, ...);
