#include <malloc.h>
#include <stdio.h>

#include "collections/gap_buffer.h"
#include "fmt.h"
#include "playground/io.h"

void print_help(void) {
    puts("commands:");
    puts("i (insert)");
    puts("d (delete)");
    puts("e (exit)");
}

int main(void) {
    struct GapBuffer *gb = gap_buffer_create_from_string("Lets go!");

    while (1) {
        gap_buffer_print(gb);
        print_newline();
        gap_buffer_debug_print(gb);

        print_help();
        print_prompt();
        char c = read_char();

        switch (c) {
        case 'i': {
            print_prompt_with_message("index");
            size_t index = read_size_t();
            print_prompt_with_message("str");
            char *s = read_str();
            gap_buffer_insert(gb, index, s);
            free(s);
            break;
        }
        case 'd': {
            print_prompt_with_message("index");
            size_t index = read_size_t();
            print_prompt_with_message("n");
            size_t n = read_size_t();

            gap_buffer_delete_n(gb, index, n);
            break;
        }
        default:
            goto EXIT;
        }
    }

EXIT:
    gap_buffer_destroy(gb);

    return 0;
}
