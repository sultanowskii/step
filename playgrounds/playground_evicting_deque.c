#include <malloc.h>
#include <stdio.h>

#include "collections/evicting_deque.h"
#include "fmt.h"
#include "playground/io.h"

void str_print(void *value) {
    char *s = value;
    printf("%s", s);
}

void str_destroy(void *s) {
    free(s);
}

void print_help(void) {
    puts("commands:");
    puts("p (push front)");
    puts("o (pop front)");
    puts("P (push back)");
    puts("O (pop back)");
    puts("e (exit)");
}

int main(void) {
    print_prompt_with_message("max_size");
    size_t max_size = read_size_t();

    struct EvictingDeque *edeque = evicting_deque_create(max_size);

    while (1) {
        evicting_deque_print(edeque, str_print);
        print_newline();
        evicting_deque_debug_print(edeque, str_print);

        print_help();
        print_prompt();
        char c = read_char();

        switch (c) {
        case 'p': {
            print_prompt_with_message("str");
            char *s = read_str();
            void *evicted = evicting_deque_push_front(edeque, s);
            str_destroy(evicted);
            break;
        }
        case 'o': {
            char *s = evicting_deque_pop_front(edeque);
            str_destroy(s);
            break;
        }
        case 'P': {
            print_prompt_with_message("str");
            char *s = read_str();
            void *evicted = evicting_deque_push_back(edeque, s);
            str_destroy(evicted);
            break;
        }
        case 'O': {
            char *s = evicting_deque_pop_back(edeque);
            str_destroy(s);
            break;
        }
        default:
            goto EXIT;
        }
    }

EXIT:
    evicting_deque_destroy(edeque, str_destroy);

    return 0;
}
