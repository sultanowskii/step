#include "tui/events/event.h"

#include <malloc.h>

#include "nonstd/runtime.h"
#include "tui/core/context.h"

enum EventType {
    EVENT_NEWLINE_ADDED,
    EVENT_NEWLINE_REMOVED,
    EVENT_KEY_UNDO,
    EVENT_KEY_REDO,
    EVENT_KEY_DELETE,
    EVENT_KEY_BACKSPACE,
    EVENT_KEY_TEXT,
    EVENT_KEY_NAVIGATION,
    EVENT_SINGULAR_NAVIGATION_REQUEST,
};

struct Event {
    enum EventType type;
    union {
        struct EventNewlineAdded              newline_added;
        struct EventNewlineRemoved            newline_removed;
        struct EventKeyUndo                   key_undo;
        struct EventKeyRedo                   key_redo;
        struct EventKeyDelete                 key_delete;
        struct EventKeyBackspace              key_backspace;
        struct EventKeyText                   key_text;
        struct EventKeyNavigation             key_navigation;
        struct EventSingularNavigationRequest singular_navigation_request;
    } body;
};

struct Event *event_create_empty(void) {
    struct Event *event = malloc(sizeof(struct Event));
    return event;
}

struct Event *event_create_newline_added(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_NEWLINE_ADDED;
    event->body.newline_added = (struct EventNewlineAdded){};
    return event;
}

struct Event *event_create_newline_removed(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_NEWLINE_REMOVED;
    event->body.newline_removed = (struct EventNewlineRemoved){};
    return event;
}

struct Event *event_create_key_undo(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_UNDO;
    event->body.key_undo = (struct EventKeyUndo){};
    return event;
}

struct Event *event_create_key_redo(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_REDO;
    event->body.key_redo = (struct EventKeyRedo){};
    return event;
}

struct Event *event_create_key_delete(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_DELETE;
    event->body.key_delete = (struct EventKeyDelete){};
    return event;
}

struct Event *event_create_key_backspace(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_BACKSPACE;
    event->body.key_backspace = (struct EventKeyBackspace){};
    return event;
}

struct Event *event_create_key_text(int key) {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_TEXT;
    event->body.key_text = (struct EventKeyText){.key = key};
    return event;
}

struct Event *event_create_key_navigation(int key) {
    struct Event *event = event_create_empty();
    event->type = EVENT_KEY_NAVIGATION;
    event->body.key_navigation = (struct EventKeyNavigation){.key = key};
    return event;
}

struct Event *event_create_singular_navigation_request(enum SingularNavigationRequest req) {
    struct Event *event = event_create_empty();
    event->type = EVENT_SINGULAR_NAVIGATION_REQUEST;
    event->body.singular_navigation_request = (struct EventSingularNavigationRequest){.req = req};
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
        case EVENT_KEY_DELETE: {
            event_handler->handle_key_delete(tctx, &(event->body.key_delete));
            break;
        }
        case EVENT_KEY_BACKSPACE: {
            event_handler->handle_key_backspace(tctx, &(event->body.key_backspace));
            break;
        }
        case EVENT_KEY_TEXT: {
            event_handler->handle_key_text(tctx, &(event->body.key_text));
            break;
        }
        case EVENT_KEY_NAVIGATION: {
            event_handler->handle_key_navigation(tctx, &(event->body.key_navigation));
            break;
        }
        case EVENT_SINGULAR_NAVIGATION_REQUEST: {
            event_handler->handle_singular_navigation_request(tctx, &(event->body.singular_navigation_request));
            break;
        }
        default: {
            panic("runtime error: unexpected command type while executing command");
        };
    }
}
