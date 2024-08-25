#pragma once

#include <stddef.h>
#include <stdio.h>

size_t file_size(FILE *f);
char  *file_read(FILE *f);
void   file_write(FILE *f, const char *s);
