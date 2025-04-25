#include "tui/keys/handle_mouse.h"

#include <ncurses.h>

#include "collections/bit_array.h"
#include "core/context.h"
#include "tui/cursor.h"

void handle_mouse(struct Context *ctx) {
    MEVENT event;
    if (getmouse(&event) != OK) {
        return;
    }

    if (event.bstate & BUTTON1_RELEASED || event.bstate & BUTTON1_CLICKED) {
        bit_array_set_at(ctx->rows_to_redraw, ctx->cursor.y);
        ctx->cursor.x = event.x - ctx->text_board->start.x;
        ctx->cursor.y = event.y - ctx->text_board->start.y;
        revise_cursor(ctx);
        bit_array_set_at(ctx->rows_to_redraw, ctx->cursor.y);
    }
}
