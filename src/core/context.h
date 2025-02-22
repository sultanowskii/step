#pragma once

#include "core/_context.h"

#include "collections/bit_array.h"
#include "collections/gap_buffer.h"
#include "core/commands/undo.h"
#include "core/state.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/events/event_queue.h"

struct Context {
    char *filepath;

    // Program state
    enum State state;
    // Event queue
    struct EventQueue *events;

    // Undo/redo storage
    struct UndoFacade *undo_facade;

    // Text buffer
    struct GapBuffer *gap_buffer;

    // Line number board
    struct Board *line_number_board;
    // Status board
    struct Board *status_board;
    // Main text board
    struct Board *text_board;
    // Current cursor position
    struct Coords cursor;

    // Index of the first symbol on the screen
    size_t starting_symbol_index;
    // Index of the first line on the screen
    size_t starting_line_index;
    // Number of lines in a buffer
    size_t line_count;

    // Lines to update the screen
    struct BitArray *rows_to_redraw;

    // Selection boundaries
    size_t selection_starting_symbol_index;
    size_t selection_ending_symbol_index;
};

// Creates context with specified done, undone stack and gap_buffer gap buffer.
struct Context *context_create(
    const char        *filepath,
    enum State         state,
    struct EventQueue *events,
    struct UndoFacade *undo_facade,
    struct GapBuffer  *gap_buffer,
    struct Board      *line_number_board,
    struct Board      *status_board,
    struct Board      *text_board,
    struct Coords      cursor,
    size_t             starting_symbol_index,
    size_t             starting_line_index,
    size_t             line_count,
    struct BitArray   *rows_to_redraw,
    size_t             selection_starting_symbol_index,
    size_t             selection_ending_symbol_index
);

// Destroys context. Some fields must be destroyed manually beforehand:
// - undo_facade
// - gap_buffer
// - line_number_board
// - status_board
// - text_board
// - cursor
void context_destroy(struct Context *ctx);
