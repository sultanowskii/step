#include "tui/keys/handle_key.h"

#include "core/context.h"
#include "tui/keys/handle_key_insert_mode.h"
#include "tui/keys/handle_key_normal_mode.h"

void handle_key(
    struct Context *ctx,
    int             key
) {
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
