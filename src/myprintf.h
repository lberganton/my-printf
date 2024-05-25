/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.h
 * Author: Lucas Berganton
 * Created: 05/25/2024
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma once

#include <stdarg.h>
#include <stdio.h>

#define vfprintf my_vfprintf
int my_vfprintf(FILE *file, const char *format, va_list args);

#define fprintf my_fprintf
int my_fprintf(FILE *file, const char *format, ...);

#define vprintf my_vprintf
int my_vprintf(const char *format, va_list args);

#define printf my_printf
int my_printf(const char *format, ...);
