#include "util.h"

#include <stdio.h>
#include <stddef.h>

void print_indent(size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf(" ");
    }
}
