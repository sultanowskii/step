#include "fmt.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

void print_indent(size_t indent_size) {
    for (size_t i = 0; i < indent_size; i++) {
        printf(" ");
    }
}

void iprintf(size_t indent_size, const char *format, ...) {
    va_list args;
    va_start(args, format);

    print_indent(indent_size);
    vprintf(format, args);

    va_end(args);
}

void print_newline(void) {
    puts("");
}
