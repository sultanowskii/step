#pragma once

#include "tui/board.h"
#include "tui/coords.h"

void print_filler(struct Board *board, const struct Coords *pos, size_t n);
void print_filler_to_eol(struct Board *board, const struct Coords *pos);
