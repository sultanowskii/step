#include "core/context.h"

#include <malloc.h>

#include "collections/bit_array.h"
#include "collections/gap_buffer.h"
#include "core/commands/undo.h"
#include "core/state.h"
#include "nonstd/str.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/events/event_queue.h"

struct Context *_context_create_empty(void) {
    struct Context *ctx = malloc(sizeof(struct Context));
    ctx->filepath = NULL;
    ctx->state = STATE_START;
    ctx->events = NULL;
    ctx->undo_facade = NULL;
    ctx->gap_buffer = NULL;
    ctx->line_number_board = NULL;
    ctx->status_board = NULL;
    ctx->text_board = NULL;
    ctx->cursor = (struct Coords){.y = 0, .x = 0};
    ctx->starting_symbol_index = 0;
    ctx->starting_line_index = 0;
    ctx->rows_to_redraw = NULL;
    ctx->selection_starting_symbol_index = 0;
    ctx->selection_ending_symbol_index = 0;
    ctx->clipboard = NULL;
    return ctx;
}

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
    size_t             selection_ending_symbol_index,
    struct Clipboard  *clipboard
) {
    struct Context *ctx = _context_create_empty();
    ctx->filepath = str_dup(filepath);
    ctx->state = state;
    ctx->events = events;
    ctx->undo_facade = undo_facade;
    ctx->gap_buffer = gap_buffer;
    ctx->line_number_board = line_number_board;
    ctx->status_board = status_board;
    ctx->text_board = text_board;
    ctx->cursor = cursor;
    ctx->starting_symbol_index = starting_symbol_index;
    ctx->starting_line_index = starting_line_index;
    ctx->line_count = line_count;
    ctx->rows_to_redraw = rows_to_redraw;
    ctx->selection_starting_symbol_index = selection_starting_symbol_index;
    ctx->selection_ending_symbol_index = selection_ending_symbol_index;
    ctx->clipboard = clipboard;
    return ctx;
}

void context_destroy(struct Context *ctx) {
    free(ctx->filepath);
    ctx->filepath = NULL;
    ctx->events = NULL;
    ctx->undo_facade = NULL;
    ctx->gap_buffer = NULL;
    ctx->line_number_board = NULL;
    ctx->status_board = NULL;
    ctx->text_board = NULL;
    ctx->cursor = (struct Coords){.y = 0, .x = 0};
    ctx->rows_to_redraw = NULL;
    ctx->selection_starting_symbol_index = 0;
    ctx->selection_ending_symbol_index = 0;
    ctx->clipboard = NULL;
    free(ctx);
}
