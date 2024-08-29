#pragma once

#include "tui/boards/board.h"

void highlight_cursor(struct Board *board, size_t y, size_t x);
void unhighlight_cursor(struct Board *board, size_t y, size_t x);

void highlight_line(struct Board *board, size_t y, int color_pair);
