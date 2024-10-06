#pragma once

#include "tui/coords.h"
#include "tui/optionals.h"

optional_coords next_valid_coords(
    const struct Coords *coords,
    size_t               max_rows,
    size_t               max_columns,
    char                 symbol
);

struct Coords next_valid_coords_without_height_limit(
    const struct Coords *coords,
    size_t               max_columns,
    char                 symbol
);
