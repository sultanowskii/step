#include "tui/tui.h"

#include <malloc.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/gap_buffer.h"
#include "io.h"
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

    size_t cursor_y = 0;
    size_t cursor_x = 0;

    int c = 0;
    while (TRUE) {
        gap_buffer_print_to_window(gb, text_window, 0, text_window_height, text_window_width);
        HIGHLIGHT_ON(text_window, cursor_y, cursor_x);
        wrefresh(text_window);

        c = wgetch(text_window);

        switch (c) {
        case KEY_RIGHT:
            if (cursor_x == text_window_width - 1) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor_y, cursor_x);
            cursor_x++;
            break;
        case KEY_LEFT:
            if (cursor_x == 0) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor_y, cursor_x);
            cursor_x--;
            break;
        case KEY_DOWN:
            if (cursor_y == text_window_height - 1) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor_y, cursor_x);
            cursor_y++;
            break;
        case KEY_UP:
            if (cursor_y == 0) {
                break;
            }
            HIGHLIGHT_OFF(text_window, cursor_y, cursor_x);
            cursor_y--;
            break;
        default:
            goto LOOP_END;
        }
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
