#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "core/context.h"
#include "tui/boards/board.h"
#include "tui/coords.h"

void update_status_board(struct Context *ctx);
