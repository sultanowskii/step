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
#include "io.h"
#include "math.h"
#include "mem.h"
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

void text(struct Context *ctx) {
    size_t window_height, window_width;
    getmaxyx(stdscr, window_height, window_width);

    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    size_t            gb_index = 0;
    size_t            gb_line_count = gap_buffer_count_lines(gb);
    size_t            gb_line_count_digit_count = count_digits(gb_line_count);

    size_t  line_number_window_height = window_height;
    size_t  line_number_window_width = gb_line_count_digit_count + 1;
    WINDOW *line_number_window = newwin(line_number_window_height, line_number_window_width, 0, 0);
    PANEL  *line_number_panel = new_panel(line_number_window);
    for (size_t i = 0; i < line_number_window_height; i++) {
        mvwprintw(panel_window(line_number_panel), i, 0, "%*zu", (int)line_number_window_width, i);
    }

    size_t  text_window_offset_x = line_number_window_width + 1;
    size_t  text_window_height = window_height;
    size_t  text_window_width = window_width - text_window_offset_x;
    WINDOW *text_window = newwin(text_window_height, text_window_width, 0, text_window_offset_x);
    keypad(text_window, TRUE);
    wattrset(text_window, COLOR_PAIR(0));
    PANEL *text_panel = new_panel(text_window);

    struct Coords cursor = {.x = 0, .y = 0};

    size_t  status_window_height = 1;
    size_t  status_window_width = window_width;
    WINDOW *status_window = newwin(status_window_height, status_window_width, window_height - 1, 0);
    // TODO: color doesnt cover whole window
    wattrset(status_window, COLOR_PAIR(1));
    PANEL *status_panel = new_panel(status_window);

    int c = 0;
    while (TRUE) {
        // Text panel update. TODO: extract into separate function
        gap_buffer_print_to_window(gb, panel_window(text_panel), gb_index, text_window_height, text_window_width);
        HIGHLIGHT_ON(panel_window(text_panel), cursor.y, cursor.x);

        // Status panel update. TODO: extract into separate function
        wclrtoeol(panel_window(status_panel));
        mvwprintw(panel_window(status_panel), 0, 0, "Cursor position: y=%zu x=%zu", cursor.y, cursor.x);

        update_panels();
        doupdate();

        c = wgetch(text_window);
        if (c == 'q') {
            goto LOOP_END;
        }

        handle_navigation_key(c, &cursor, text_window, text_window_height, text_window_width);

        struct Coords revised = gap_buffer_revise_coords(gb, gb_index, text_window_height, text_window_width, cursor);
        cursor.y = revised.y;
        cursor.x = revised.x;
    }

LOOP_END:
    del_panel(line_number_panel);
    delwin(line_number_window);
    del_panel(text_panel);
    delwin(text_window);
    del_panel(status_panel);
    delwin(status_window);
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
