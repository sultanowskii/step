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
    keypad(stdscr, true);
}

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

    WINDOW *text_window = newwin(window_height, window_width, 0, 0);

    gap_buffer_print_to_window(gb, text_window, 0, window_height, window_width);
    wgetch(text_window);

    delwin(text_window);
}

void main_window(void) {
    setup();

    text();

    printw("Press any key to exit...");
    getch();

    teardown();
}
