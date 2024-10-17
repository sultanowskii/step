#pragma once

#include <stddef.h>

// Prints indent of the given size.
void print_indent(size_t size);
// Same as printf, but prints indent before.
void iprintf(size_t indent_size, const char *format, ...);
// Prints a newline.
void print_newline(void);
// Same as sprintf, but allocates a buffer.
char *alloc_sprintf(const char *format, ...);
