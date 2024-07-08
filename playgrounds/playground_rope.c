#include "collections/rope.h"

#include <stdio.h>

int main() {
    char data[1024 + 6];
    setvbuf(stdout, NULL, _IONBF, 0);

    struct Rope *rope = rope_create_from_string("Hello, world!");
    rope_print(rope);
    puts("");
    rope_debug_print(rope);

    struct RopePair pair = rope_split(rope, 3);
    rope_print(pair.first);
    puts("");
    rope_destroy(pair.first);

    rope_print(pair.second);
    puts("");

    struct Rope *loworld = rope_delete(pair.second, 2, 4);
    rope_print(loworld);
    puts("");

    struct Rope *loba_world = rope_insert(loworld, 2, "ba_");
    rope_print(loba_world);
    puts("");

    rope_destroy(loba_world);

    struct Rope *long_text = rope_create_from_string("VERY\nVERY...\nVery!\nLong text! It's\nridiculous how long it is!");
    char buffer[1024];
    size_t long_text_symbols_written = rope_fill_buffer_from_index(long_text, buffer, 3, 1000, 6);
    buffer[long_text_symbols_written] = '\0';
    printf("Here: %s\n", buffer);
    rope_destroy(long_text);

    struct Rope *text = rope_create_from_string("pple");
    text = rope_insert(text, 0, "A");

    while (1) {
        puts("text:");
        rope_print(text);
        puts("");
        puts("commands:");
        puts("i(insert)");
        puts("d(elete)");
        puts("g(et)");
        puts("e(xit)");
        printf("> ");

        char c;
        scanf(" %c", &c);

        if (c == 'i') {
            int index;
            puts("index");
            printf("> ");
            scanf(" %d", &index);
            puts("str");
            printf("> ");
            scanf(" %1024s", data);
            text = rope_insert(text, index, data);
        } else if (c == 'd') {
            int l, r;
            puts("l");
            printf("> ");
            scanf(" %d", &l);
            puts("r");
            printf("> ");
            scanf(" %d", &r);

            text = rope_delete(text, l, r);
        } else if (c == 'g') {
            int index;
            puts("index");
            printf("> ");
            scanf(" %d", &index);

            char c = rope_get_at(text, index);
            printf("%c\n", c);
        } else if (c == 'e') {
            break;
        }
    }

    rope_destroy(text);

    return 0;
}
