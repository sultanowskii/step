#include "nonstd/fmt.h"

#include <malloc.h>
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

char *alloc_sprintf(const char *format, ...) {
    va_list args_to_determine_size;
    va_start(args_to_determine_size, format);
    size_t formatted_size = vsnprintf(NULL, 0, format, args_to_determine_size);
    va_end(args_to_determine_size);

    va_list args_to_format;
    va_start(args_to_format, format);
    char *s = malloc(formatted_size + 1);
    vsprintf(s, format, args_to_format);
    va_end(args_to_format);

    return s;
}
