#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

size_t file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

char *file_read(FILE *f) {
    size_t size = file_size(f);

    char *buf = malloc(size + 1);
    fread(buf, size, 1, f);
    buf[size] = '\0';

    return buf;
}
