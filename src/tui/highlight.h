#pragma once

#include "tui/boards/board.h"

void highlight_on(struct Board *board, size_t y, size_t x);
void highlight_off(struct Board *board, size_t y, size_t x);
