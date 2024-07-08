#include <ncurses.h>
#include <stdbool.h>

void setup() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
}

void teardown() {
    endwin();
}

void main_window() {
    setup();

    printw("Hello, world! Press the button!\n");

    char c = getch();

    printw("Pressed key %c!\n", c);
    refresh();

    printw("Press any key to exit...");
    getch();

    teardown();
}
