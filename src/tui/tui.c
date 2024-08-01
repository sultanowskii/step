#include "tui/tui.h"

#include <malloc.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/gap_buffer.h"
#include "io.h"
#include "tui/coords.h"
#include "tui/text.h"

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

void text(void) {
    size_t window_height, window_width;
    getmaxyx(stdscr, window_height, window_width);

    FILE *f = fopen("Makefile", "rb");
    char *data = file_read(f);
    fclose(f);

    struct GapBuffer *gb = gap_buffer_create_from_string(data);
    free(data);

    size_t  text_window_height = window_height;
    size_t  text_window_width = window_width;
    WINDOW *text_window = newwin(text_window_height, text_window_width, 0, 0);
    keypad(text_window, TRUE);

    struct Coords cursor = {.x = 0, .y = 0};

    size_t gb_index = 0;

    int c = 0;
    while (TRUE) {
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

        struct Coords actual_coords = gap_buffer_revise_coords(gb, gb_index, text_window_height, text_window_width, cursor.y, cursor.x);
        cursor.y = actual_coords.y;
        cursor.x = actual_coords.x;
    }

LOOP_END:
    delwin(text_window);
}

void main_window(void) {
    setup();

    text();

    printw("Press any key to exit...");
    getch();

    teardown();
}
