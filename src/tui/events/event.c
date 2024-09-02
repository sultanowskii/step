#include "tui/events/event.h"

#include <malloc.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "nonstd/runtime.h"
#include "tui/core/context.h"

#include "tui/layout.h"

enum EventType {
    EVENT_NEWLINE_ADDED,
    EVENT_NEWLINE_REMOVED,
};

struct EventNewlineAdded {
    size_t prev_line_count;
};

struct EventNewlineRemoved {
    size_t prev_line_count;
};

struct Event {
    enum EventType type;
    union {
        struct EventNewlineAdded   newline_added;
        struct EventNewlineRemoved newline_removed;
    } body;
};

struct Event *event_create_empty() {
    struct Event *event = malloc(sizeof(struct Event));
    return event;
}

struct Event *event_create_newline_added(size_t prev_line_count) {
    struct Event *event = event_create_empty();
    event->type = EVENT_NEWLINE_ADDED;
    event->body.newline_added = (struct EventNewlineAdded){
        .prev_line_count = prev_line_count,
    };
    return event;
}

struct Event *event_create_newline_removed(size_t prev_line_count) {
    struct Event *event = event_create_empty();
    event->type = EVENT_NEWLINE_REMOVED;
    event->body.newline_removed = (struct EventNewlineRemoved){
        .prev_line_count = prev_line_count,
    };
    return event;
}

void event_destroy(struct Event *event) {
    free(event);
}

void event_handle_newline_added(struct TuiContext *tctx, struct EventNewlineAdded *newline_added) {
    size_t prev_line_count_digits = count_digits(newline_added->prev_line_count);

    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));
    size_t current_line_count_digits = count_digits(current_line_count);

    if (current_line_count_digits != prev_line_count_digits) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }
}

void event_handle_newline_removed(struct TuiContext *tctx, struct EventNewlineRemoved *newline_removed) {
    size_t prev_line_count_digits = count_digits(newline_removed->prev_line_count);

    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));
    size_t current_line_count_digits = count_digits(current_line_count);

    if (current_line_count_digits != prev_line_count_digits) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }
}

void event_handle(struct TuiContext *tctx, struct Event *event) {
    switch (event->type) {
        case EVENT_NEWLINE_ADDED: {
            event_handle_newline_added(tctx, &(event->body.newline_added));
            break;
        };
        case EVENT_NEWLINE_REMOVED: {
            event_handle_newline_removed(tctx, &(event->body.newline_removed));
            break;
        };
        default: {
            panic("runtime error: unexpected command type while executing command");
        };
    }
}
