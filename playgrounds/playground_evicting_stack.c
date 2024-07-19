#include <malloc.h>
#include <stdio.h>

#include "collections/evicting_stack.h"

void str_print(void *value) {
    char *s = value;
    printf("%s", s);
}

void str_destroy(void *s) {
    free(s);
}

int main() {
    const size_t str_size = 128;

    int max_size;
    puts("max_size");
    printf("> ");
    scanf(" %d", &max_size);
    struct EvictingStack *estack = evicting_stack_create(max_size);

    while (1) {
        evicting_stack_print(estack, str_print);
        puts("");
        evicting_stack_debug_print(estack, str_print);
        puts("commands:");
        puts("P (push back)");
        puts("O (pop back)");
        puts("e (exit)");
        printf("> ");

        char c;
        scanf(" %c", &c);

        switch (c) {
        case 'P': {
            char *s = malloc(str_size);
            puts("str");
            printf("> ");
            scanf(" %127s", s);
            void *value = evicting_stack_push_back(estack, s);
            str_destroy(value);
            break;
        }
        case 'O': {
            void *value = evicting_stack_pop_back(estack);
            str_destroy(value);
            break;
        }
        default:
            goto EXIT;
        }
    }

EXIT:
    evicting_stack_destroy(estack, str_destroy);

    return 0;
}
