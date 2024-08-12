#include "tui/keys/delete_key.h"

#include <ncurses.h>

#include "core/commands.h"
#include "tui/context.h"
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
            if (index == gb_length - 1) {
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
        default:
            return;
    }

    struct Command *cmd_delete = command_create_delete(index, 1);

    command_exec(tctx->ctx, cmd_delete);

    move_cursor_to_index(tctx, text_board->height, text_board->width, index);
}