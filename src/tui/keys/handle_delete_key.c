#include "tui/keys/handle_delete_key.h"

#include <ncurses.h>

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "core/commands/commands.h"
#include "tui/core/context.h"
#include "tui/optionals.h"
#include "tui/text.h"

// TODO: return value
optional_char handle_delete_key(struct TuiContext *tctx, struct Board *text_board, int key) {
    optional_size_t maybe_index = get_index_from_cursor_position(tctx, text_board->height, text_board->width);
    if (size_t_is_none(maybe_index)) {
        return char_none();
    }

    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);
    size_t            index = size_t_get_val(maybe_index);

    switch (key) {
        case KEY_DC: {
            size_t gb_length = gap_buffer_get_length(gb);
            if (index >= gb_length) {
                return char_none();
            }
            break;
        }
        case KEY_BACKSPACE: {
            if (index == 0) {
                return char_none();
            }
            index--;
            break;
        }
        default: {
            return char_none();
        }
    }

    char symbol = gap_buffer_get_at(gb, index);

    struct Command       *cmd = command_create_delete_symbol(index);
    struct CommandResult *result = command_exec(tctx->ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(tui_context_get_undo_facade(tctx), result);

    move_cursor_to_index(tctx, text_board->height, text_board->width, index);

    return char_some(symbol);
}
