#include <ncurses.h>

#include <stddef.h>

#include "tui/coords.h"

const char FILLER = 'a';

void print_info(WINDOW *win, size_t y, size_t x) {
    mvwprintw(win, 0, 0, "Press F1 to quit y=%zu x=%zu", y, x);
}

void funny(void) {
    size_t window_height, window_width;
    getmaxyx(stdscr, window_height, window_width);

    size_t  info_window_height = 1;
    size_t  info_window_width = window_width;
    WINDOW *info_window = newwin(info_window_height, info_window_width, 0, 0);

    size_t  funny_window_height = window_height - info_window_height;
    size_t  funny_window_width = window_width;
    WINDOW *funny_window = newwin(funny_window_height, funny_window_width, 1, 0);
    keypad(funny_window, true);

    int           ch;
    struct Coords cursor = {.y = 0, .x = 0};

    print_info(info_window, cursor.y, cursor.x);
    wmove(funny_window, cursor.y, cursor.x);
    wrefresh(info_window);

    while ((ch = wgetch(funny_window)) != KEY_F(1)) {
        switch (ch) {
        case KEY_LEFT:
            if (cursor.x == 0) {
                goto REFRESH;
            }
            cursor.x--;
            break;
        case KEY_RIGHT:
            if (cursor.x == funny_window_width - 1) {
                goto REFRESH;
            }
            cursor.x++;
            break;
        case KEY_UP:
            if (cursor.y == 0) {
                goto REFRESH;
            }
            cursor.y--;
            break;
        case KEY_DOWN:
            if (cursor.y == funny_window_height - 1) {
                goto REFRESH;
            }
            cursor.y++;
            break;
        }

        int sym = mvwinch(funny_window, cursor.y, cursor.x);
        if (sym == FILLER) {
            sym = ' ';
        } else {
            sym = FILLER;
        }
        mvwaddch(funny_window, cursor.y, cursor.x, sym);

    REFRESH:
        print_info(info_window, cursor.y, cursor.x);
        wrefresh(info_window);

        wmove(funny_window, cursor.y, cursor.x);
        wrefresh(funny_window);
    }

    delwin(info_window);
    delwin(funny_window);
}

int main(void) {
    initscr();
    raw();
    noecho();

    funny();

    endwin();
    return 0;
}