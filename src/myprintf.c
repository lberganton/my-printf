/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File: myprintf.c
 * Author: Lucas Berganton
 * Created: 05/25/2024
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "myprintf.h"
#include <stdio.h>

static inline int char_out(FILE *file, int ch) {
  fwrite(&ch, sizeof(int), 1, file);
}
