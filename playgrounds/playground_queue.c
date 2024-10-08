#include <malloc.h>
#include <stdio.h>

#include "collections/queue.h"
#include "nonstd/fmt.h"
#include "playground/io.h"

void str_print(const void *value) {
    const char *s = value;
    printf("%s", s);
}

void str_destroy(void *s) {
    free(s);
}

void print_help(void) {
    puts("commands:");
    puts("p (push)");
    puts("o (pop)");
    puts("e (exit)");
}

int main(void) {
    struct Queue *queue = queue_create();

    while (1) {
        queue_print(queue, str_print);
        print_newline();
        queue_debug_print(queue, str_print);

        print_help();
        print_prompt();
        char c = read_char();

        switch (c) {
            case 'p': {
                print_prompt_with_message("str");
                char *s = read_str();
                queue_push(queue, s);
                break;
            }
            case 'o': {
                char *s = queue_pop(queue);
                str_destroy(s);
                break;
            }

            default:
                goto EXIT;
        }
    }

EXIT:
    queue_destroy(queue, str_destroy);

    return 0;
}
