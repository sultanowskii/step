#include "tui/keys/handle_key.h"

#include "core/context.h"
#include "tui/cursor.h"
#include "tui/keys/handle_key_insert_mode.h"
#include "tui/keys/handle_key_normal_mode.h"
#include "tui/boards/board.h"
#include "tui/layout.h"

void handle_mouse(struct Context *ctx) {
    MEVENT event;
    if (getmouse(&event) != OK) {
        return;
    }

    if (event.bstate & BUTTON1_RELEASED || event.bstate & BUTTON1_CLICKED) {
        // TODO: move outta here?
        ctx->cursor.x = event.x - ctx->text_board->start.x;
        ctx->cursor.y = event.y - ctx->text_board->start.y;
        revise_cursor(ctx);
    }
}

void handle_key(struct Context *ctx, int key) {
    switch (key) {
        case KEY_RESIZE: {
            recompose_boards(ctx);
            return;
        }
        if (key == KEY_MOUSE) {
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
        case STATE_EXIT: {
            break;
        }
    }
}
