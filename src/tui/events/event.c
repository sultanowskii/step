#include "tui/events/event.h"

#include <malloc.h>

#include "nonstd/runtime.h"
#include "tui/core/context.h"

enum EventType {
    EVENT_NEWLINE_ADDED,
    EVENT_NEWLINE_REMOVED,
    EVENT_KEY_UNDO,
    EVENT_KEY_REDO,
};

struct Event {
    enum EventType type;
    union {
        struct EventNewlineAdded   newline_added;
        struct EventNewlineRemoved newline_removed;
        struct EventKeyUndo        key_undo;
        struct EventKeyRedo        key_redo;
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

struct Event *event_create_key_undo() {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_UNDO;
    event->body.key_undo = (struct EventKeyUndo){};
    return event;
}

struct Event *event_create_key_redo() {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_REDO;
    event->body.key_redo = (struct EventKeyRedo){};
    return event;
}

void event_destroy(struct Event *event) {
    free(event);
}

void event_handle(const struct EventHandler *event_handler, struct TuiContext *tctx, struct Event *event) {
    switch (event->type) {
        case EVENT_NEWLINE_ADDED: {
            event_handler->handle_newline_added(tctx, &(event->body.newline_added));
            break;
        };
        case EVENT_NEWLINE_REMOVED: {
            event_handler->handle_newline_removed(tctx, &(event->body.newline_removed));
            break;
        };
        case EVENT_KEY_UNDO: {
            event_handler->handle_key_undo(tctx, &(event->body.key_undo));
            break;
        }
        case EVENT_KEY_REDO: {
            event_handler->handle_key_redo(tctx, &(event->body.key_redo));
            break;
        }
        default: {
            panic("runtime error: unexpected command type while executing command");
        };
    }
}
