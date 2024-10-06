#pragma once

#include <stdbool.h>

#include "core/context.h"
#include "nonstd/optionals.h"

void revise_cursor(struct Context *ctx);

bool move_cursor_to_index(struct Context *ctx, size_t target_index);

optional_size_t get_index_from_cursor_position(const struct Context *ctx);
optional_size_t get_line_index_from_cursor(const struct Context *ctx);

size_t first_y_of_line_under_cursor(struct Context *ctx);
size_t last_y_of_line_under_cursor(struct Context *ctx);
