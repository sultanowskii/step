#pragma once

#include <stdbool.h>

#include "tui/boards/board.h"
#include "tui/coords.h"

void print_filler(struct Board *board, const struct Coords *pos, size_t n);
void print_filler_till_end_of_row(struct Board *board, const struct Coords *pos);
void print_filler_till_end_of_board(struct Board *board, const struct Coords *pos);
