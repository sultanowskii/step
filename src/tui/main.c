#include "tui/main.h"

#include <errno.h>
#include <malloc.h>
#include <ncurses.h>
#include <panel.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "core/commands/undo.h"
#include "core/context.h"
#include "core/state.h"
#include "nonstd/io.h"
#include "nonstd/math.h"
#include "nonstd/runtime.h"
#include "tui/boards/board.h"
#include "tui/boards/line_number_board.h"
#include "tui/boards/status_board.h"
#include "tui/boards/text_board.h"
#include "tui/color.h"
#include "tui/coords.h"
#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/events/event_queue.h"
#include "tui/handlers/delete.h"
#include "tui/handlers/navigation.h"
#include "tui/handlers/newline.h"
#include "tui/handlers/text.h"
#include "tui/handlers/undo.h"
#include "tui/keys/handle_key.h"
#include "tui/layout.h"
#include "tui/text.h"
#include "tui/tui.h"

void setup(void) {
    initscr();
    setup_colors();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void teardown(void) {
    endwin();
}

void loop(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *status_board,
    struct Board   *text_board
) {
    struct Coords      cursor = {.y = 0, .x = 0};
    const size_t       starting_symbol_index = 0;
    const size_t       starting_line_index = 0;
    struct EventQueue *event_queue = event_queue_create();

    struct TuiContext *tctx = tui_context_create(
        ctx,
        event_queue,
        line_number_board,
        status_board,
        text_board,
        starting_symbol_index,
        starting_line_index,
        &cursor
    );

    struct EventHandler event_handler = {
        .handle_newline_added = handle_newline_added,
        .handle_newline_removed = handle_newline_removed,
        .handle_key_undo = handle_key_undo,
        .handle_key_redo = handle_key_redo,
        .handle_key_delete = handle_key_delete,
        .handle_key_backspace = handle_key_backspace,
        .handle_key_text = handle_key_text,
        .handle_key_navigation = handle_key_navigation,
        .handle_singular_navigation_request = handle_singular_navigation_request,
    };

    while (context_get_state(ctx) != STATE_EXIT) {
        update_line_number_board(tctx, text_board->height, text_board->width);

        update_text_board(tctx);

        update_status_board(tctx);

        update_panels();
        doupdate();

        int sym = wgetch(board_window(text_board));
        handle_key(tctx, sym);

        while (!event_queue_is_empty(event_queue)) {
            struct Event *event = event_queue_pop(event_queue);
            event_handle(&event_handler, tctx, event);
        }

        revise_cursor(tctx, text_board->height, text_board->width);
    }

    tui_context_destroy(tctx);
    event_queue_destroy(event_queue);
}

// TODO: rename
void run(struct Context *ctx) {
    struct Board *line_number_board = board_create_dummy();
    struct Board *status_board = board_create_dummy();
    struct Board *text_board = board_create_dummy();

    recompose_boards(ctx, line_number_board, status_board, text_board);

    loop(ctx, line_number_board, status_board, text_board);

    board_destroy(line_number_board);
    board_destroy(text_board);
    board_destroy(status_board);
}

struct Context *setup_context(const char *filename) {
    enum State state = STATE_START;

    struct UndoFacade *undo_facade = undo_facade_create();

    FILE *f = fopen(filename, "rb");
    // TODO: handle properly
    if (f == NULL) {
        fprintf(stderr, "failed to open file '%s': %s\n", filename, strerror(errno));
        panic("TODO: remove/replace me!");
    }
    char *data = file_read(f);
    fclose(f);
    struct GapBuffer *gb = gap_buffer_create_from_string(data);
    free(data);

    struct Context *ctx = context_create(state, undo_facade, gb, filename);

    undo_facade_set_ctx(undo_facade, ctx);

    return ctx;
}

void teardown_context(struct Context *ctx) {
    struct UndoFacade *undo_facade = context_get_undo_facade(ctx);
    undo_facade_destroy(undo_facade);

    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    gap_buffer_destroy(gb);

    context_destroy(ctx);
}

void tui_main(const char *filename) {
    struct Context *ctx = setup_context(filename);
    setup();

    run(ctx);

    teardown();
    teardown_context(ctx);
}
