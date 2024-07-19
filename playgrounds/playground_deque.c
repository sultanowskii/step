#include "collections/deque.h"

#include <malloc.h>
#include <stdio.h>

void str_print(void *value) {
    char *s = value;
    printf("%s", s);
}

void str_destroy(void *s) {
    free(s);
}

int main() {
    const size_t  str_size = 128;
    struct Deque *deque = deque_create();

    while (1) {
        puts("text:");
        deque_print(deque, str_print);
        puts("");
        deque_debug_print(deque, str_print);
        puts("commands:");
        puts("p (push front)");
        puts("o (pop front)");
        puts("P (push back)");
        puts("O (pop back)");
        puts("e (exit)");
        printf("> ");

        char c;
        scanf(" %c", &c);

        switch (c) {
        case 'p': {
            char *s = malloc(str_size);
            puts("str");
            printf("> ");
            scanf(" %127s", s);
            puts("starting");
            deque_push_front(deque, s);
            puts("ended");
            break;
        }
        case 'o': {
            char *s = deque_pop_front(deque);
            str_destroy(s);
            break;
        }
        case 'P': {
            char *s = malloc(str_size);
            puts("str");
            printf("> ");
            scanf(" %127s", s);
            deque_push_back(deque, s);
            break;
        }
        case 'O': {
            char *s = deque_pop_back(deque);
            str_destroy(s);
            break;
        }
        default:
            goto EXIT;
        }
    }

EXIT:
    deque_destroy(deque, str_destroy);

    return 0;
}
