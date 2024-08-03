#include <malloc.h>
#include <stdio.h>

#include "collections/rope.h"
#include "fmt.h"
#include "playground/io.h"

void print_help(void) {
    puts("commands:");
    puts("i (insert)");
    puts("d (delete)");
    puts("g (get at)");
    puts("e (exit)");
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    struct Rope *rope = rope_create_from_string("Hello, world!");
    rope_print(rope);
    print_newline();
    rope_debug_print(rope);

    struct RopePair pair = rope_split(rope, 3);
    rope_print(pair.first);
    print_newline();
    rope_destroy(pair.first);

    rope_print(pair.second);
    print_newline();

    struct Rope *loworld = rope_delete(pair.second, 2, 4);
    rope_print(loworld);
    print_newline();

    struct Rope *loba_world = rope_insert(loworld, 2, "ba_");
    rope_print(loba_world);
    print_newline();

    rope_destroy(loba_world);

    struct Rope *long_text = rope_create_from_string("VERY\nVERY...\nVery!\nLong gap_buffer! It's\nridiculous how long it is!");
    char         buffer[1024];
    size_t       long_text_symbols_written = rope_fill_buffer_from_index(long_text, buffer, 3, 1000, 6);
    buffer[long_text_symbols_written] = '\0';
    printf("Here: %s\n", buffer);
    rope_destroy(long_text);

    struct Rope *gap_buffer = rope_create_from_string("pple");
    gap_buffer = rope_insert(gap_buffer, 0, "A");

    while (1) {
        rope_print(gap_buffer);
        print_newline();

        print_help();
        print_prompt();
        char c = read_char();

        if (c == 'i') {
            print_prompt_with_message("index");
            size_t index = read_size_t();
            print_prompt_with_message("str");
            char *s = read_str();
            gap_buffer = rope_insert(gap_buffer, index, s);
            free(s);
        } else if (c == 'd') {
            print_prompt_with_message("l");
            size_t l = read_size_t();
            print_prompt_with_message("r");
            size_t r = read_size_t();

            gap_buffer = rope_delete(gap_buffer, l, r);
        } else if (c == 'g') {
            print_prompt_with_message("index");
            size_t index = read_size_t();

            char c = rope_get_at(gap_buffer, index);
            printf("%c\n", c);
        } else if (c == 'e') {
            break;
        }
    }

    rope_destroy(gap_buffer);

    return 0;
}
