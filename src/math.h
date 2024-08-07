#pragma once

#include <stddef.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define INVALID_SIZE_T ((size_t) - 1)

// Returns number of digits in specified number
// (aka 'number length').
size_t count_digits(size_t n);
