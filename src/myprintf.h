/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.h
 * Author: Lucas Berganton
 * Created: 05/25/2024
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma once

#include <stdarg.h>
#include <stdio.h>

#define MY_PRINTF_DEFAULT_PRECISION 6
#define MY_PRINTF_MAX_PRECISION 16

int my_vfprintf(FILE *file, const char *format, va_list args);

int my_fprintf(FILE *file, const char *format, ...);

int my_vprintf(const char *format, va_list args);

int my_printf(const char *format, ...);
