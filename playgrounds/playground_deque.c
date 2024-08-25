#include <malloc.h>
#include <stdio.h>

#include "collections/deque.h"
#include "nonstd/fmt.h"
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
    struct Deque *deque = deque_create();

    while (1) {
        deque_print(deque, str_print);
        print_newline();
        deque_debug_print(deque, str_print);

        print_help();
        print_prompt();
        char c = read_char();

        switch (c) {
            case 'p': {
                print_prompt_with_message("str");
                char *s = read_str();
                deque_push_front(deque, s);
                break;
            }
            case 'o': {
                char *s = deque_pop_front(deque);
                str_destroy(s);
                break;
            }
            case 'P': {
                print_prompt_with_message("str");
                char *s = read_str();
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
