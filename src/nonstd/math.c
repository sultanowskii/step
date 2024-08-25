#include "nonstd/math.h"

#include <stddef.h>

size_t count_digits(size_t n) {
    size_t counter = 0;
    while (n != 0) {
        n /= 10;
        counter++;
    }
    return counter;
}
