#include "collections/gap_buffer_str.h"

#include <stddef.h>
#include <string.h>

#include "collections/gap_buffer.h"
#include "math.h"

size_t gap_buffer_count_symbol(const struct GapBuffer *gb, char symbol) {
    size_t counter = 0;
    for (size_t i = gap_buffer_first_index(gb); i != INVALID_SIZE_T; i = gap_buffer_next_index(gb, i)) {
        if (gap_buffer_get_at(gb, i) == symbol) {
            counter++;
        }
    }

    return counter;
}

size_t gap_buffer_count_lines(const struct GapBuffer *gb) {
    return 1 + gap_buffer_count_symbol(gb, '\n');
}