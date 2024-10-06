#include "tui/next_coords.h"

#include "tui/coords.h"
#include "tui/optionals.h"

optional_coords next_valid_coords(
    const struct Coords *coords,
    size_t               max_rows,
    size_t               max_columns,
    char                 symbol
) {
    struct Coords next = *coords;

    next.x++;
    if (next.x == max_columns || symbol == '\n') {
        next.x = 0;
        next.y++;
    }
    if (next.y == max_rows) {
        return coords_none();
    }

    return coords_some(next);
}

struct Coords next_valid_coords_without_height_limit(
    const struct Coords *coords,
    size_t               max_columns,
    char                 symbol
) {
    struct Coords next = *coords;

    next.x++;
    if (next.x == max_columns || symbol == '\n') {
        next.x = 0;
        next.y++;
    }

    return next;
}
