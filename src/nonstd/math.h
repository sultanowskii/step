#pragma once

#include <stddef.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define UNSIGNED_ABS_DIFF(x, y) ((x) > (y)) ? ((x) - (y)) : ((y) - (x))

#define INVALID_SIZE_T ((size_t)-1)

#define SWAP(a, b)         \
    do {                   \
        typeof(a) tmp = a; \
        a = b;             \
        b = tmp;           \
    } while (0);

// Returns number of digits in specified number
// (aka 'number length').
size_t count_digits(size_t n);
