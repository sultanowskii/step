#include "tui/keys/handle_key.h"

#include "collections/bit_array.h"
#include "core/context.h"
#include "tui/boards/board.h"
#include "tui/cursor.h"
#include "tui/keys/handle_key_insert_mode.h"
#include "tui/keys/handle_key_normal_mode.h"
#include "tui/keys/handle_key_visual_mode.h"
#include "tui/layout.h"

// TODO: move to handlers/
void handle_mouse(struct Context *ctx) {
    MEVENT event;
    if (getmouse(&event) != OK) {
        return;
    }

    if (event.bstate & BUTTON1_RELEASED || event.bstate & BUTTON1_CLICKED) {
        // TODO: move outta here?
        bit_array_set_at(ctx->rows_to_redraw, ctx->cursor.y);
        ctx->cursor.x = event.x - ctx->text_board->start.x;
        ctx->cursor.y = event.y - ctx->text_board->start.y;
        revise_cursor(ctx);
        bit_array_set_at(ctx->rows_to_redraw, ctx->cursor.y);
    }
}

void handle_key(struct Context *ctx, int key) {
    switch (key) {
        case KEY_RESIZE: {
            bit_array_flood(ctx->rows_to_redraw);
            recompose_boards(ctx);
            return;
        }
        case KEY_MOUSE: {
            handle_mouse(ctx);
            return;
        }
    }

    switch (ctx->state) {
        case STATE_START: {
            break;
        }
        case STATE_INSERT: {
            handle_key_insert_mode(ctx, key);
            break;
        }
        case STATE_NORMAL: {
            handle_key_normal_mode(ctx, key);
            break;
        }
        case STATE_VISUAL: {
            handle_key_visual_mode(ctx, key);
            break;
        }
        case STATE_EXIT: {
            break;
        }
    }
}
