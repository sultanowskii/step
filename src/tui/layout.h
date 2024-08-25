#pragma once

#include "core/context.h"
#include "tui/boards/board.h"

// Recomposes (calculates and sets size to) all boards.
void recompose_boards(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *status_board,
    struct Board   *text_board
);
