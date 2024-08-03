#include "tui/tui.h"

#include <malloc.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "core/context.h"
#include "core/state.h"
#include "io.h"
#include "mem.h"
#include "tui/coords.h"
#include "tui/text.h"

// TODO: move to other file
#define UNDO_MAX_COUNT 50

void setup(void) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

#define HIGHLIGHT_ON(win, y, x) mvwchgat(win, y, x, 1, A_REVERSE, 0, NULL)
#define HIGHLIGHT_OFF(win, y, x) mvwchgat(win, y, x, 1, A_NORMAL, 0, NULL)

void teardown(void) {
    endwin();
}

void text(struct Context *ctx) {
    size_t window_height, window_width;
    getmaxyx(stdscr, window_height, window_width);

    size_t  text_window_height = window_height;
    size_t  text_window_width = window_width;
    WINDOW *text_window = newwin(text_window_height, text_window_width, 0, 0);
    keypad(text_window, TRUE);

    struct Coords cursor = {.x = 0, .y = 0};

    size_t gb_index = 0;

    int c = 0;
    while (TRUE) {
        struct GapBuffer *gb = context_get_gap_buffer(ctx);
        gap_buffer_print_to_window(gb, text_window, gb_index, text_window_height, text_window_width);
        HIGHLIGHT_ON(text_window, cursor.y, cursor.x);
        wrefresh(text_window);

        c = wgetch(text_window);

        switch (c) {
        case KEY_RIGHT:
            if (cursor.x == text_window_width - 1) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor.y, cursor.x);
            cursor.x++;
            break;
        case KEY_LEFT:
            if (cursor.x == 0) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor.y, cursor.x);
            cursor.x--;
            break;
        case KEY_DOWN:
            if (cursor.y == text_window_height - 1) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor.y, cursor.x);
            cursor.y++;
            break;
        case KEY_UP:
            if (cursor.y == 0) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor.y, cursor.x);
            cursor.y--;
            break;
        default:
            goto LOOP_END;
        }

        struct Coords revised = gap_buffer_revise_coords(gb, gb_index, text_window_height, text_window_width, cursor);
        cursor.y = revised.y;
        cursor.x = revised.x;
    }

LOOP_END:
    delwin(text_window);
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
