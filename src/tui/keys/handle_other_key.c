#include "tui/keys/handle_other_key.h"

#include <stdbool.h>

#include "core/commands/commands.h"
#include "nonstd/optional.h"
#include "tui/boards/board.h"
#include "tui/core/context.h"
#include "tui/optionals.h"
#include "tui/text.h"

DECLARE_OPTIONAL(char, char)

bool is_key_printable(int symbol) {
    return ' ' <= symbol && symbol <= '~';
}

optional_char convert_key_to_symbol(int key) {
    if (is_key_printable(key)) {
        return char_some(key);
    }

    switch (key) {
        case '\n':
        case KEY_ENTER: {
            return char_some('\n');
        }
        default: {
            break;
        }
    }

    return char_none();
}

void handle_other(
    struct TuiContext *tctx,
    struct Board      *text_board,
    int                key
) {
    optional_char maybe_symbol = convert_key_to_symbol(key);
    if (char_is_none(maybe_symbol)) {
        return;
    }

    char symbol = char_get_val(maybe_symbol);

    optional_size_t maybe_index = get_index_from_cursor_position(tctx, text_board->height, text_board->width);
    if (size_t_is_none(maybe_index)) {
        return;
    }
    size_t index = size_t_get_val(maybe_index);

    // TODO: extract it all into separate function
    struct Command       *cmd = command_create_insert_symbol(index, symbol);
    struct CommandResult *result = command_exec(tctx->ctx, cmd);
    command_destroy(cmd);
    struct EvictingStack *done = tui_context_get_done_cmds(tctx);
    struct CommandResult *evicted_result = evicting_stack_push_back(done, result);
    command_result_destroy(evicted_result);

    index++;
    move_cursor_to_index(tctx, text_board->height, text_board->width, index);
}
