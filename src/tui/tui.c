#include "tui/tui.h"

#include <ncurses.h>
#include <stddef.h>

#include "tui/color.h"

void tui_setup(void) {
    initscr();
    setup_colors();

    // direct input
    raw();

    // disable echoing user input
    noecho();

    // enable keypad
    keypad(stdscr, TRUE);

    // hide cursor (we manage it ourselves)
    curs_set(0);

    // ESCDELAY is responsible for a delay after hitting Escape
    // in order to distinguish it from escape sequence
    //
    // https://en.wikipedia.org/wiki/Escape_sequence
    // man 3 ESCDELAY
    set_escdelay(10);

    mousemask(
        BUTTON1_PRESSED | BUTTON1_RELEASED | BUTTON1_CLICKED | BUTTON_SHIFT | BUTTON_CTRL | REPORT_MOUSE_POSITION,
        NULL
    );
}

void tui_teardown(void) {
    endwin();
}