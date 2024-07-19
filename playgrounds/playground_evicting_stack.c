#include <malloc.h>
#include <stdio.h>

#include "collections/evicting_stack.h"
#include "fmt.h"
#include "playground/io.h"

void str_print(void *value) {
    char *s = value;
    printf("%s", s);
}

void str_destroy(void *s) {
    free(s);
}

void print_help() {
    puts("commands:");
    puts("P (push back)");
    puts("O (pop back)");
    puts("e (exit)");
}

int main() {
    print_prompt_with_message("max_size");
    size_t max_size = read_size_t();

    struct EvictingStack *estack = evicting_stack_create(max_size);

    while (1) {
        evicting_stack_print(estack, str_print);
        print_newline();
        evicting_stack_debug_print(estack, str_print);

        print_help();
        print_prompt();
        char c = read_char();

        switch (c) {
        case 'P': {
            print_prompt_with_message("str");
            char *s = read_str();
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
