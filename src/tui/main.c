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
#include "core/commands.h"
#include "core/context.h"
#include "core/state.h"
#include "io.h"
#include "math.h"
#include "mem.h"
#include "runtime.h"
#include "tui/boards/board.h"
#include "tui/boards/line_number_board.h"
#include "tui/boards/status_board.h"
#include "tui/boards/text_board.h"
#include "tui/context.h"
#include "tui/coords.h"
#include "tui/keys/handle_key.h"
#include "tui/layout.h"
#include "tui/text.h"
#include "tui/tui.h"

// TODO: move to other file
#define UNDO_MAX_COUNT 50

// TODO: custom colors
void setup_color_pairs() {
    use_default_colors();
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
}

void setup(void) {
    initscr();
    setup_color_pairs();
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
    struct Board   *text_board,
    struct Board   *status_board
) {
    struct Coords      cursor = {.y = 0, .x = 0};
    const size_t       starting_symbol_index = 0;
    const size_t       starting_line_index = 0;
    struct TuiContext *tctx = tui_context_create(ctx, starting_symbol_index, starting_line_index, &cursor);

    while (context_get_state(ctx) != STATE_EXIT) {
        update_line_number_board(tctx, line_number_board, text_board->height, text_board->width);

        update_text_board(tctx, text_board);

        update_status_board(tctx, text_board, status_board);

        update_panels();
        doupdate();

        int sym = wgetch(board_window(text_board));
        handle_key(tctx, line_number_board, status_board, text_board, sym);

        revise_cursor(tctx, text_board->height, text_board->width);
    }

    tui_context_destroy(tctx);
}

// TODO: rename
void run(struct Context *ctx) {
    struct Board *line_number_board = board_create_dummy();
    struct Board *status_board = board_create_dummy();
    struct Board *text_board = board_create_dummy();

    recompose_boards(ctx, line_number_board, status_board, text_board);

    loop(ctx, line_number_board, text_board, status_board);

    board_destroy(line_number_board);
    board_destroy(text_board);
    board_destroy(status_board);
}

struct Context *setup_context(const char *filename) {
    enum State            state = STATE_START;
    struct EvictingStack *done_cmds = evicting_stack_create(UNDO_MAX_COUNT);
    struct EvictingStack *undone_cmds = evicting_stack_create(UNDO_MAX_COUNT);

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

    struct Context *ctx = context_create(state, done_cmds, undone_cmds, gb, filename);
    return ctx;
}

void teardown_context(struct Context *ctx) {
    struct EvictingStack *done_cmds = context_get_done_cmds(ctx);
    evicting_stack_destroy(done_cmds, command_result_destroy);

    struct EvictingStack *undone_cmds = context_get_undone_cmds(ctx);
    evicting_stack_destroy(undone_cmds, command_result_destroy);

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
