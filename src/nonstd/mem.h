#pragma once

#include <stddef.h>

#define FREED_DUMMY (void *)0xEEEEEEEEEEEEEEEE

// Destroyer function type which is used for dynamic memory cleanup
//
// If provided pointer is NULL, it should do nothing
typedef void (*func_destroy)(void *);

// Dummy function that satisfies `func_destroy` type
// Does nothing
void destroy_dummy(void *);

size_t grow_size(size_t size);
