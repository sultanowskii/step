#include "tui/keys/handle_delete_key.h"

#include <ncurses.h>

#include "collections/evicting_stack.h"
#include "core/commands/commands.h"
#include "tui/core/context.h"
#include "tui/optionals.h"
#include "tui/text.h"

void handle_delete_key(struct TuiContext *tctx, struct Board *text_board, int key) {
    optional_size_t maybe_index = get_index_from_cursor_position(tctx, text_board->height, text_board->width);
    if (size_t_is_none(maybe_index)) {
        return;
    }

    size_t index = size_t_get_val(maybe_index);

    switch (key) {
        case KEY_DC: {
            size_t gb_length = gap_buffer_get_length(tui_context_get_gap_buffer(tctx));
            if (index >= gb_length) {
                return;
            }
            break;
        }
        case KEY_BACKSPACE: {
            if (index == 0) {
                return;
            }
            index--;
            break;
        }
        default: {
            return;
        }
    }

    // TODO: extract it all into separate function
    struct Command       *cmd = command_create_delete_symbol(index);
    struct CommandResult *result = command_exec(tctx->ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(tui_context_get_undo_facade(tctx), result);

    move_cursor_to_index(tctx, text_board->height, text_board->width, index);
}