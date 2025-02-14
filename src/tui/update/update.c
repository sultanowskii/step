#include "tui/update/update.h"

#include "collections/bit_array.h"
#include "tui/cursor.h"

#include <stddef.h>

void require_redraw_for_line_at_cursor(struct Context *ctx) {
    size_t first_row = first_y_of_line_under_cursor(ctx);
    size_t last_row = last_y_of_line_under_cursor(ctx);
    for (size_t y = first_row; y <= last_row; y++) {
        bit_array_set_at(ctx->rows_to_redraw, y);
    }
}
