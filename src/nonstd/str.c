#include "nonstd/str.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

char *str_dup(const char *orig) {
    size_t n = strlen(orig);
    char  *s = malloc(n + 1);
    strncpy(s, orig, n + 1);
    s[n] = '\0';
    return s;
}

char *str_slice(const char *orig, size_t l, size_t r) {
    size_t n = r - l;
    char  *s = malloc(n + 1);
    strncpy(s, orig + l, n);
    s[n] = '\0';
    return s;
}

size_t str_count(const char *s, char c) {
    size_t result = 0;

    for (size_t i = 0; i < strlen(s); i++) {
        if (s[i] == c) {
            result++;
        }
    }

    return result;
}
