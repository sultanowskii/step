#include "collections/gap_buffer_str.h"

#include <stddef.h>
#include <string.h>

#include "collections/gap_buffer.h"

size_t gap_buffer_count_symbol(const struct GapBuffer *gb, char symbol) {
    size_t counter = 0;

    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = 0; i < gb_length; i++) {
        if (gap_buffer_get_at(gb, i) == symbol) {
            counter++;
        }
    }

    return counter;
}

size_t gap_buffer_count_lines(const struct GapBuffer *gb) {
    return 1 + gap_buffer_count_symbol(gb, '\n');
}
