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
#include "core/context.h"
#include "core/state.h"
#include "io.h"
#include "math.h"
#include "mem.h"
#include "runtime.h"
#include "tui/board.h"
#include "tui/context.h"
#include "tui/coords.h"
#include "tui/line_number_board.h"
#include "tui/navigation.h"
#include "tui/status_board.h"
#include "tui/text.h"
#include "tui/text_board.h"
#include "tui/tui.h"

// TODO: move to other file
#define UNDO_MAX_COUNT 50

void recreate_boards(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *status_board,
    struct Board   *text_board
);

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
    const size_t       buf_starting_symbol_index = 0;
    const size_t       buf_starting_line_index = 0;
    struct TuiContext *tctx = tui_context_create(ctx, buf_starting_symbol_index, buf_starting_line_index, &cursor);

    while (true) {
        update_line_number_board(tctx, line_number_board, text_board->height, text_board->width);

        update_text_board(tctx, text_board);

        update_status_board(tctx, status_board);

        update_panels();
        doupdate();

        int c = wgetch(board_window(text_board));
        if (c == 'q') {
            break;
        }

        if (c == KEY_RESIZE) {
            recreate_boards(tctx->ctx, line_number_board, status_board, text_board);
        }

        enum NavigationRequest request = handle_navigation_key(tctx, text_board, c);
        if (request != NAVREQ_NO) {
            fulfill_navigation_request(tctx, request);
        }

        revise_cursor(tctx, text_board->height, text_board->width);
    }

    tui_context_destroy(tctx);
}

// TODO: rename
void recreate_boards(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *status_board,
    struct Board   *text_board
) {
    size_t window_height, window_width;
    getmaxyx(stdscr, window_height, window_width);

    const size_t gb_line_count = gap_buffer_count_lines(context_get_gap_buffer(ctx));
    const size_t gb_line_count_digit_count = count_digits(gb_line_count);

    const size_t line_number_window_height = window_height;
    const size_t line_number_window_width = gb_line_count_digit_count + 1;
    board_resize(line_number_board, line_number_window_height, line_number_window_width, 0, 0);

    const size_t status_window_height = 1;
    const size_t status_window_width = window_width;
    board_resize(status_board, status_window_height, status_window_width, window_height - 1, 0);
    WINDOW *status_window = board_window(status_board);
    wattrset(status_window, COLOR_PAIR(1));

    const size_t text_window_offset_x = line_number_board->width + 1;
    const size_t text_window_height = window_height - status_window_height;
    const size_t text_window_width = window_width - text_window_offset_x;
    board_resize(text_board, text_window_height, text_window_width, 0, text_window_offset_x);
    WINDOW *text_window = board_window(text_board);
    keypad(text_window, TRUE);
    wattrset(text_window, COLOR_PAIR(0));
}

// TODO: rename
void run(struct Context *ctx) {
    struct Board *line_number_board = board_create_dummy();
    struct Board *status_board = board_create_dummy();
    struct Board *text_board = board_create_dummy();

    recreate_boards(ctx, line_number_board, status_board, text_board);

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

    struct Context *ctx = context_create(state, done_cmds, undone_cmds, gb);
    return ctx;
}

void teardown_context(struct Context *ctx) {
    struct EvictingStack *done_cmds = context_get_done_cmds(ctx);
    // TODO: replace destroy_dummy with appropriate destroyer
    evicting_stack_destroy(done_cmds, destroy_dummy);

    struct EvictingStack *undone_cmds = context_get_undone_cmds(ctx);
    // TODO: replace destroy_dummy with appropriate destroyer
    evicting_stack_destroy(undone_cmds, destroy_dummy);

    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    gap_buffer_destroy(gb);

    context_destroy(ctx);
}

void tui_main(const char *filename) {
    struct Context *ctx = setup_context(filename);

    setup();

    run(ctx);

    printw("Press any key to exit...");
    getch();

    teardown();

    teardown_context(ctx);
}
