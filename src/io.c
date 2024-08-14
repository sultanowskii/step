#include "io.h"

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

size_t file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

char *file_read(FILE *f) {
    size_t size = file_size(f);

    char *buf = malloc(size + 1);
    if (size > 0) {
        fread(buf, size, 1, f);
    }
    buf[size] = '\0';

    return buf;
}

void file_write(FILE *f, const char *s) {
    // TODO: handle case when less chunks written?
    fwrite(s, sizeof(char), strlen(s), f);
}
