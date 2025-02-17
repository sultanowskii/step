#include "tui/handlers/text.h"

#include <stdbool.h>

#include "core/context.h"
#include "nonstd/optionals.h"
#include "tui/cursor.h"
#include "tui/events/event.h"

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

void handle_key_text(struct Context *ctx, const struct EventKeyText *key_text) {
    optional_char maybe_symbol = convert_key_to_symbol(key_text->key);
    if (char_is_none(maybe_symbol)) {
        return;
    }

    char symbol = char_get_val(maybe_symbol);

    optional_size_t maybe_index = get_index_from_cursor_position(ctx);
    if (size_t_is_none(maybe_index)) {
        return;
    }
    size_t index = size_t_get_val(maybe_index);
    size_t index_of_inserted_symbol = index;

    struct Command       *cmd = command_create_insert_symbol(index, symbol);
    struct CommandResult *result = command_exec(ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(ctx->undo_facade, result);

    index++;
    move_cursor_to_index(ctx, index);

    event_queue_push_symbol_added(ctx->events, index_of_inserted_symbol, symbol);

    // TODO: optimize?
    bit_array_flood(ctx->rows_to_redraw);
}
