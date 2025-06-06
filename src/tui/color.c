#include "tui/color.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>

size_t rgb_to_ncurses(size_t rgb) {
    return rgb * 1000 / 255;
}

// Shortcut for `rgb_to_ncurses()`
static inline size_t _(size_t rgb) {
    return rgb_to_ncurses(rgb);
}

void setup_custom_colors(void) {
    init_color(COLOR_TEXT_TEXT, _(215), _(215), _(215));
    init_color(COLOR_TEXT_BACKGROUND, _(40), _(40), _(40));
    init_pair(COLOR_PAIR_TEXT, COLOR_TEXT_TEXT, COLOR_TEXT_BACKGROUND);

    init_color(COLOR_TEXT_HIGHLIGHTED_TEXT, _(215), _(215), _(215));
    init_color(COLOR_TEXT_HIGHLIGHTED_BACKGROUND, _(50), _(50), _(50));
    init_pair(COLOR_PAIR_TEXT_HIGHLIGHTED, COLOR_TEXT_HIGHLIGHTED_TEXT, COLOR_TEXT_HIGHLIGHTED_BACKGROUND);

    init_color(COLOR_TEXT_CURSOR_TEXT, _(40), _(40), _(40));
    init_color(COLOR_TEXT_CURSOR_BACKGROUND, _(215), _(215), _(215));
    init_pair(COLOR_PAIR_TEXT_CURSOR, COLOR_TEXT_CURSOR_TEXT, COLOR_TEXT_CURSOR_BACKGROUND);

    init_color(COLOR_TEXT_SELECTION_TEXT, _(40), _(40), _(40));
    init_color(COLOR_TEXT_SELECTION_BACKGROUD, _(190), _(190), _(190));
    init_pair(COLOR_PAIR_TEXT_SELECTION, COLOR_TEXT_SELECTION_TEXT, COLOR_TEXT_SELECTION_BACKGROUD);

    init_color(COLOR_LINE_NUMBER_TEXT, _(120), _(120), _(120));
    init_color(COLOR_LINE_NUMBER_BACKGROUND, _(40), _(40), _(40));
    init_pair(COLOR_PAIR_LINE_NUMBER, COLOR_LINE_NUMBER_TEXT, COLOR_LINE_NUMBER_BACKGROUND);

    init_color(COLOR_LINE_NUMBER_HIGHLIGHTED_TEXT, _(215), _(215), _(215));
    init_color(COLOR_LINE_NUMBER_HIGHLIGHTED_BACKGROUND, _(40), _(40), _(40));
    init_pair(COLOR_PAIR_LINE_NUMBER_HIGHLIGHTED, COLOR_LINE_NUMBER_HIGHLIGHTED_TEXT, COLOR_LINE_NUMBER_HIGHLIGHTED_BACKGROUND);

    init_color(COLOR_STATUS_TEXT, _(40), _(40), _(40));
    init_color(COLOR_STATUS_BACKGROUND, _(215), _(215), _(215));
    init_pair(COLOR_PAIR_STATUS, COLOR_STATUS_TEXT, COLOR_STATUS_BACKGROUND);
}

void setup_default_colors(void) {
    use_default_colors();

    init_pair(COLOR_PAIR_TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_TEXT_HIGHLIGHTED, COLOR_WHITE, COLOR_BLACK);

    init_pair(COLOR_PAIR_TEXT_CURSOR, COLOR_BLACK, COLOR_WHITE);

    init_pair(COLOR_PAIR_LINE_NUMBER, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_LINE_NUMBER_HIGHLIGHTED, COLOR_WHITE, COLOR_BLACK);

    init_pair(COLOR_PAIR_STATUS, COLOR_BLACK, COLOR_WHITE);
}

bool setup_colors(void) {
    if (!has_colors()) {
        return false;
    }

    start_color();

    if (can_change_color()) {
        setup_custom_colors();
    } else {
        setup_default_colors();
    }
    return true;
}
