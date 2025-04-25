#include "tui/handlers/copypaste.h"

#include <malloc.h>
#include <string.h>

#include "collections/gap_buffer.h"
#include "core/clipboard.h"
#include "nonstd/str.h"
#include "tui/cursor.h"

void _put_to_clipboard(struct Context *ctx, size_t index, size_t length) {
    char  *buffer = calloc(length + 1, sizeof(char));
    size_t copied = gap_buffer_strncpy_from(ctx->gap_buffer, buffer, index, length);

    clipboard_set(ctx->clipboard, buffer, copied);

    free(buffer);
}

void handle_key_copy(struct Context *ctx, const struct EventKeyCopy *key_copy) {
    _put_to_clipboard(ctx, key_copy->index, key_copy->length);
    bit_array_flood(ctx->rows_to_redraw);
}

void handle_key_cut(struct Context *ctx, const struct EventKeyCut *key_cut) {
    size_t index = key_cut->index;
    size_t length = key_cut->length;

    _put_to_clipboard(ctx, key_cut->index, key_cut->length);

    struct Command       *cmd = command_create_delete_string(index, length);
    struct CommandResult *result = command_exec(ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(ctx->undo_facade, result);

    move_cursor_to_index(ctx, key_cut->index);

    bit_array_flood(ctx->rows_to_redraw);
}

void handle_key_paste(struct Context *ctx, const struct EventKeyPaste *key_paste) {
    size_t index = key_paste->index;

    const char *s = clipboard_get(ctx->clipboard);
    size_t      length = strlen(s);
    if (length == 0) {
        return;
    }

    struct Command       *cmd = command_create_insert_string(index, s, length);
    struct CommandResult *result = command_exec(ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(ctx->undo_facade, result);

    move_cursor_to_index(ctx, key_paste->index + length);
    bit_array_flood(ctx->rows_to_redraw);
}

void handle_request_delete_string(struct Context *ctx, const struct EventRequestDeleteString *request_delete_string) {
    size_t index = request_delete_string->index;
    size_t length = request_delete_string->length;

    struct Command       *cmd = command_create_delete_string(index, length);
    struct CommandResult *result = command_exec(ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(ctx->undo_facade, result);

    move_cursor_to_index(ctx, request_delete_string->index);

    bit_array_flood(ctx->rows_to_redraw);
}
