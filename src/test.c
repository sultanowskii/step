#include "rope.h"

#include <stdio.h>

int main() {
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

    return 0;
}
