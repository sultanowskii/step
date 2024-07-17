#pragma once

#include <stddef.h>

char *str_dup(const char *orig);
char *str_slice(const char *orig, size_t l, size_t r);
char *str_copy(char *dst, const char *src, size_t max_n);
