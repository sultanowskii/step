#pragma once

#include <stddef.h>

void print_indent(size_t size);
void iprintf(size_t indent_size, const char *format, ...);
void print_newline();
