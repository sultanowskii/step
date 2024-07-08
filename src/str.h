#pragma once

#include <stddef.h>

char *strdup(const char *orig);
char *strslice(const char *orig, size_t l, size_t r);

char *str_copy(char *dst, const char *src, size_t max_n);
