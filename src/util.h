#pragma once

#include <stddef.h>

#define FREED_DUMMY (void *)0xEEEEEEEEEEEEEEEE

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void print_indent(size_t size);
