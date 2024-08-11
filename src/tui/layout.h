#pragma once

#include "core/context.h"
#include "tui/board.h"

void recreate_boards(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *status_board,
    struct Board   *text_board
);
