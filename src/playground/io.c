#include "playground/io.h"

#include <malloc.h>
#include <stddef.h>

void print_prompt() {
    printf("> ");
}

void print_prompt_with_message(const char *msg) {
    puts(msg);
    print_prompt();
}

size_t read_size_t() {
    size_t v;
    scanf(" %zu", &v);
    return v;
}

char *read_str() {
    char *s = malloc(128);
    scanf(" %127s", s);
    return s;
}

char read_char() {
    char c;
    scanf(" %c", &c);
    return c;
}
