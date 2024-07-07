#include "debug.h"

#include <stddef.h>
#include <stdio.h>

void print_indent(size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf(" ");
    }
}
