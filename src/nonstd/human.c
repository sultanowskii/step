#include "nonstd/human.h"

#include <stddef.h>

size_t index_to_human(size_t index) {
    return index + 1;
}

size_t index_from_human(size_t human) {
    return human - 1;
}
