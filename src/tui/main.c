#include "tui/main.h"

#include <errno.h>
#include <malloc.h>
#include <ncurses.h>
#include <panel.h>
#include <string.h>

#include "collections/bit_array.h"
#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "core/clipboard.h"
#include "core/commands/undo.h"
#include "core/context.h"
#include "core/error_message.h"
#include "core/state.h"
#include "nonstd/fmt.h"
#include "nonstd/io.h"
#include "tui/boards/board.h"
#include "tui/boards/line_number_board.h"
#include "tui/boards/status_board.h"
#include "tui/boards/text_board.h"
#include "tui/coords.h"
#include "tui/cursor.h"
#include "tui/events/event.h"
#include "tui/events/event_queue.h"
#include "tui/handlers/copypaste.h"
#include "tui/handlers/handlers.h"
#include "tui/keys/handle_key.h"
#include "tui/layout.h"
#include "tui/tui.h"

void loop(struct Context *ctx) {
    struct EventHandler event_handler = {
        .handle_symbol_added = handle_symbol_added,
        .handle_symbol_removed = handle_symbol_removed,
        .handle_key_undo = handle_key_undo,
        .handle_key_redo = handle_key_redo,
        .handle_key_delete = handle_key_delete,
        .handle_key_backspace = handle_key_backspace,
        .handle_key_text = handle_key_text,
        .handle_key_copy = handle_key_copy,
        .handle_key_cut = handle_key_cut,
        .handle_key_paste = handle_key_paste,
        .handle_request_delete_string = handle_request_delete_string,
        .handle_key_navigation = handle_key_navigation,
        .handle_request_go_up = handle_request_go_up,
        .handle_request_go_down = handle_request_go_down,
        .handle_request_go_to_bof = handle_request_go_to_bof,
        .handle_request_go_to_eof = handle_request_go_to_eof,
    };

    struct Board      *text_board = ctx->text_board;
    struct EventQueue *event_queue = ctx->events;

    ctx->state = STATE_NORMAL;

    while (ctx->state != STATE_EXIT) {
        update_line_number_board(ctx);
        update_text_board(ctx);
        update_status_board(ctx);

        update_panels();
        doupdate();

        bit_array_clear(ctx->rows_to_redraw);

        int sym = wgetch(board_window(text_board));
        handle_key(ctx, sym);

        while (!event_queue_is_empty(event_queue)) {
            struct Event *event = event_queue_pop(event_queue);
            event_handle(&event_handler, ctx, event);
            revise_cursor(ctx);
            event_destroy(event);
        }
    }
}

void run(struct Context *ctx) {
    recompose_boards(ctx);
    loop(ctx);
}

struct Context *context_setup(const char *filename) {
    enum State state = STATE_START;

    struct UndoFacade *undo_facade = undo_facade_create();

    FILE *f = fopen(filename, "a+");
    if (f == NULL) {
        error_message = alloc_sprintf("failed to open file '%s': %s", filename, strerror(errno));
        return NULL;
    }
    char *data = file_read(f);
    fclose(f);
    struct GapBuffer *gb = gap_buffer_create_from_string(data);
    free(data);

    size_t line_count = gap_buffer_count_lines(gb);

    struct Coords      cursor = {.y = 0, .x = 0};
    const size_t       starting_symbol_index = 0;
    const size_t       starting_line_index = 0;
    const size_t       selection_starting_symbol_index = 0;
    const size_t       selection_ending_symbol_index = 0;
    struct EventQueue *event_queue = event_queue_create();
    struct BitArray   *rows_to_redraw = bit_array_create(256);
    bit_array_flood(rows_to_redraw);

    struct Board *line_number_board = board_create_dummy();
    struct Board *status_board = board_create_dummy();
    struct Board *text_board = board_create_dummy();

    struct Clipboard *clipboard = clipboard_create();

    struct Context *ctx = context_create(
        filename,
        state,
        event_queue,
        undo_facade,
        gb,
        line_number_board,
        status_board,
        text_board,
        cursor,
        starting_symbol_index,
        starting_line_index,
        line_count,
        rows_to_redraw,
        selection_starting_symbol_index,
        selection_ending_symbol_index,
        clipboard
    );

    undo_facade_set_ctx(undo_facade, ctx);

    return ctx;
}

void context_teardown(struct Context *ctx) {
    undo_facade_destroy(ctx->undo_facade);

    clipboard_destroy(ctx->clipboard);

    bit_array_destroy(ctx->rows_to_redraw);

    gap_buffer_destroy(ctx->gap_buffer);

    board_destroy(ctx->line_number_board);
    board_destroy(ctx->text_board);
    board_destroy(ctx->status_board);

    event_queue_destroy(ctx->events);

    context_destroy(ctx);
}

bool tui_main(const char *filename) {
    bool success = true;

    tui_setup();

    struct Context *ctx = context_setup(filename);
    if (ctx == NULL) {
        success = false;
        goto TUI_TEARDOWN;
    }

    run(ctx);

    context_teardown(ctx);
TUI_TEARDOWN:
    tui_teardown();
    return success;
}
