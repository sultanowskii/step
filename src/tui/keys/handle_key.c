#include "tui/keys/handle_key.h"

#include "core/context.h"
#include "tui/boards/board.h"
#include "tui/keys/handle_key_insert_mode.h"
#include "tui/keys/handle_key_normal_mode.h"
#include "tui/keys/handle_key_visual_mode.h"
#include "tui/keys/handle_mouse.h"
#include "tui/layout.h"

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
