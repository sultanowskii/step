#pragma once

#define FREED_DUMMY (void *)0xEEEEEEEEEEEEEEEE

// Destroyer function type which is used for dynamic memory cleanup
//
// If provided pointer is NULL, it should do nothing
typedef void (*destroy)(void *);

// Dummy function that satisfies `destroy` type
// Does nothing
void destroy_dummy(void *);
