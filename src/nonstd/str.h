#pragma once

#include <stddef.h>

char  *str_dup(const char *orig);
char  *str_slice(const char *orig, size_t l, size_t r);
size_t str_count(const char *s, char c);
