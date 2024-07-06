#include "str.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

char *strdup(const char *orig) {
    size_t n = strlen(orig);
    char *s = malloc(strlen(orig) + 1);
    strncpy(s, orig, n);
    s[n] = '\0';
    return s;
}

char *strslice(const char *orig, size_t l, size_t r) {
    size_t n = r - l;
    char *s = malloc(n + 1);
    strncpy(s, orig + l, n);
    s[n] = '\0';
    return s;
}
