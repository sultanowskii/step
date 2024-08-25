#include "nonstd/runtime.h"

#include <stdio.h>
#include <stdlib.h>

void panic(const char *message) {
    puts(message);
    exit(1);
}
