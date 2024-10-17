#include "nonstd/mem.h"

#include <stddef.h>

const size_t threshold = 256;

void destroy_dummy(void *v) {
    (void)v;
}

size_t grow_size(size_t size) {
    if (size < threshold) {
        return size * 2;
    }

    return size + (size + 3 * threshold) / 4;
}
