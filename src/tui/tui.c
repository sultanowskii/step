#include "tui/tui.h"

#include <malloc.h>
#include <ncurses.h>
#include <panel.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "core/context.h"
#include "core/state.h"
#include "human.h"
#include "io.h"
#include "math.h"
#include "mem.h"
#include "tui/board.h"
#include "tui/coords.h"
#include "tui/highlight.h"
#include "tui/navigation.h"
#include "tui/text.h"

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

void run(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *text_board,
    struct Board   *status_board
) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    size_t            gb_index = 0;

    struct Coords cursor = {.x = 0, .y = 0};

    while (TRUE) {
        // Line number panel update. TODO: extract into separate function
        for (size_t i = 0; i < line_number_board->height; i++) {
            mvwprintw(board_window(line_number_board), i, 0, "%*zu", (int)line_number_board->width, i);
        }

        // Text panel update. TODO: extract into separate function
        gap_buffer_print_to_window(gb, board_window(text_board), gb_index, text_board->height, text_board->width);
        highlight_on(text_board, cursor.y, cursor.x);

        // Status panel update. TODO: extract into separate function
        wclrtoeol(board_window(status_board));
        mvwprintw(
            board_window(status_board),
            0,
            0,
            "Cursor position: Ln %zu, Col %zu (y=%zu, x=%zu)",
            index_to_human(cursor.y),
            index_to_human(cursor.x),
            cursor.y,
            cursor.x
        );

        update_panels();
        doupdate();

        int c = wgetch(board_window(text_board));
        if (c == 'q') {
            break;
        }

        handle_navigation_key(c, &cursor, text_board);

        struct Coords revised = gap_buffer_revise_coords(gb, gb_index, text_board->height, text_board->width, cursor);
        cursor.y = revised.y;
        cursor.x = revised.x;
    }
}

// TODO: rename
void text(struct Context *ctx) {
    size_t window_height, window_width;
    getmaxyx(stdscr, window_height, window_width);

    size_t gb_line_count = gap_buffer_count_lines(context_get_gap_buffer(ctx));
    size_t gb_line_count_digit_count = count_digits(gb_line_count);

    const size_t line_number_window_height = window_height;
    const size_t line_number_window_width = gb_line_count_digit_count + 1;
    struct Board line_number_board = create_board(line_number_window_height, line_number_window_width, 0, 0);

    const size_t text_window_offset_x = line_number_board.width + 1;
    const size_t text_window_height = window_height;
    const size_t text_window_width = window_width - text_window_offset_x;
    struct Board text_board = create_board(text_window_height, text_window_width, 0, text_window_offset_x);
    WINDOW      *text_window = panel_window(text_board.panel);
    keypad(text_window, TRUE);
    wattrset(text_window, COLOR_PAIR(0));

    const size_t status_window_height = 1;
    const size_t status_window_width = window_width;
    struct Board status_board = create_board(status_window_height, status_window_width, window_height - 1, 0);
    WINDOW      *status_window = panel_window(status_board.panel);
    // TODO: color doesn't cover whole window
    wattrset(status_window, COLOR_PAIR(1));

    run(ctx, &line_number_board, &text_board, &status_board);

    board_destroy(&line_number_board);
    board_destroy(&text_board);
    board_destroy(&status_board);
}

struct Context *setup_context() {
    enum State            state = STATE_START;
    struct EvictingStack *done_cmds = evicting_stack_create(UNDO_MAX_COUNT);
    struct EvictingStack *undone_cmds = evicting_stack_create(UNDO_MAX_COUNT);

    FILE *f = fopen("Makefile", "rb");
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

void main_window(void) {
    setup();

    struct Context *ctx = setup_context();

    text(ctx);

    printw("Press any key to exit...");
    getch();

    teardown();
}
