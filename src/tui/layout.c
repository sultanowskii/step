#include "tui/layout.h"

#include <ncurses.h>

#include "collections/gap_buffer_str.h"
#include "core/context.h"
#include "nonstd/math.h"
#include "tui/boards/board.h"
#include "tui/color.h"

// TODO: make configurable
#define MIN_LINE_NUMBER_BOARD_DIGIT_COUNT 1

// TODO: extract?
void recompose_boards(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *status_board,
    struct Board   *text_board
) {
    size_t window_height, window_width;
    getmaxyx(stdscr, window_height, window_width);

    const size_t gb_line_count_digit_count = gap_buffer_calculate_line_count_digit_count(context_get_gap_buffer(ctx));

    const size_t line_number_window_height = window_height;
    const size_t line_number_window_width = MAX(gb_line_count_digit_count, MIN_LINE_NUMBER_BOARD_DIGIT_COUNT) + 1;
    board_resize(line_number_board, line_number_window_height, line_number_window_width, 0, 0);
    board_set_color_pair(line_number_board, COLOR_PAIR_LINE_NUMBER);

    const size_t status_window_height = 1;
    const size_t status_window_width = window_width;
    board_resize(status_board, status_window_height, status_window_width, window_height - 1, 0);
    board_set_color_pair(status_board, COLOR_PAIR_STATUS);

    const size_t text_window_offset_x = line_number_board->width + 1;
    const size_t text_window_height = window_height - status_window_height;
    const size_t text_window_width = window_width - text_window_offset_x;
    board_resize(text_board, text_window_height, text_window_width, 0, text_window_offset_x);
    WINDOW *text_window = board_window(text_board);
    keypad(text_window, TRUE);
    board_set_color_pair(text_board, COLOR_PAIR_TEXT);
}
