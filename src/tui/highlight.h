#pragma once

#include "tui/boards/board.h"

typedef void (*highlight_func)(struct Board *, size_t y, size_t x);

void highlight_cursor(struct Board *board, size_t y, size_t x);
void unhighlight_cursor(struct Board *board, size_t y, size_t x);

void highlight_selection(struct Board *board, size_t y, size_t x);

void highlight_row(struct Board *board, size_t y, int color_pair);
