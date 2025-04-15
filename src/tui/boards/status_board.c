#include "tui/boards/status_board.h"

#include <ncurses.h>
#include <stddef.h>
#include <string.h>

#include "core/context.h"
#include "core/state.h"
#include "nonstd/human.h"
#include "nonstd/optionals.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/cursor.h"
#include "tui/fmt.h"
#include "tui/text.h"

void print_err(WINDOW *status_board_window) {
    wprintw(status_board_window, "(err)");
}

void print_value_state(struct Context *ctx) {
    const char *state_str = STATE_STR[ctx->state];
    wprintw(board_window(ctx->status_board), "%s", state_str);
    print_filler(ctx->status_board, STATE_STR_MAX_LENGTH - strlen(state_str));
}

void print_value_ln(struct Context *ctx) {
    WINDOW *status_board_window = board_window(ctx->status_board);

    optional_size_t maybe_line_index = get_line_index_from_cursor(ctx);
    if (size_t_is_none(maybe_line_index)) {
        print_err(status_board_window);
        return;
    }

    size_t line_index = size_t_get_val(maybe_line_index);
    wprintw(status_board_window, "%zu", index_to_human(line_index));
}

void print_value_col(struct Context *ctx) {
    WINDOW *status_board_window = board_window(ctx->status_board);

    optional_size_t maybe_cursor_index = get_index_from_cursor_position(ctx);
    if (size_t_is_none(maybe_cursor_index)) {
        print_err(status_board_window);
        return;
    }
    size_t index = size_t_get_val(maybe_cursor_index);

    optional_size_t maybe_line_start_index = find_start_of_current_line(ctx->gap_buffer, index);
    if (size_t_is_none(maybe_cursor_index)) {
        print_err(status_board_window);
        return;
    }

    size_t col = index - size_t_get_val(maybe_line_start_index);
    wprintw(status_board_window, "%zu", index_to_human(col));
}

void print_value_file_size(struct Context *ctx) {
    WINDOW *status_board_window = board_window(ctx->status_board);
    wprintw(status_board_window, "%zu", gap_buffer_get_length(ctx->gap_buffer));
}

void print_value_index(struct Context *ctx) {
    WINDOW *status_board_window = board_window(ctx->status_board);

    optional_size_t maybe_index = get_index_from_cursor_position(ctx);
    if (size_t_is_none(maybe_index)) {
        print_err(status_board_window);
        return;
    }

    size_t index = size_t_get_val(maybe_index);
    wprintw(status_board_window, "%zu", index);
}

void print_selection(struct Context *ctx) {
    WINDOW *status_board_window = board_window(ctx->status_board);

    wprintw(status_board_window, "[%zu; %zu]", ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index);
}

void update_status_board(struct Context *ctx) {
    struct Board *status_board = ctx->status_board;
    WINDOW       *status_board_window = board_window(status_board);

    wclrtoeol(status_board_window);
    wmove(status_board_window, 0, 0);

    print_value_state(ctx);

    wprintw(status_board_window, " ");
    print_value_ln(ctx);

    wprintw(status_board_window, ":");
    print_value_col(ctx);

    if (0) {
        wprintw(status_board_window, ". File size: ");
        print_value_file_size(ctx);
    }

    wprintw(status_board_window, ". Index: ");
    print_value_index(ctx);

    if (ctx->state == STATE_VISUAL) {
        wprintw(status_board_window, " Selection: ");
        print_selection(ctx);
    }

    struct Coords current = {
        .y = getcury(status_board_window),
        .x = getcurx(status_board_window),
    };

    print_filler_till_end_of_board(status_board, &current);
}
