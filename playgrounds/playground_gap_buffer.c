#include "collections/gap_buffer.h"

#include <stdio.h>

int main() {
    struct GapBuffer *gb = gap_buffer_create_from_string("Lets go!");
    char              data[1024 + 6];

    while (1) {
        puts("text:");
        gap_buffer_print(gb);
        puts("");
        gap_buffer_debug_print(gb);
        puts("commands:");
        puts("i(insert)");
        puts("d(elete)");
        puts("e(xit)");
        printf("> ");

        char c;
        scanf(" %c", &c);

        switch (c) {
        case 'i': {
            int index;
            puts("index");
            printf("> ");
            scanf(" %d", &index);
            puts("str");
            printf("> ");
            scanf(" %1024s", data);
            gap_buffer_insert(gb, index, data);
            break;
        }
        case 'd': {
            int index, n;
            puts("index");
            printf("> ");
            scanf(" %d", &index);
            puts("n");
            printf("> ");
            scanf(" %d", &n);

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
