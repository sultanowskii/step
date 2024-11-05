#include "tui/events/event.h"

#include <malloc.h>

#include "core/context.h"
#include "nonstd/runtime.h"

enum EventType {
    EVENT_SYMBOL_ADDED,
    EVENT_SYMBOL_REMOVED,
    EVENT_KEY_UNDO,
    EVENT_KEY_REDO,
    EVENT_KEY_DELETE,
    EVENT_KEY_BACKSPACE,
    EVENT_KEY_TEXT,
    EVENT_KEY_NAVIGATION,
    EVENT_REQUEST_GO_UP,
    EVENT_REQUEST_GO_DOWN,
    EVENT_REQUEST_GO_TO_BOF,
    EVENT_REQUEST_GO_TO_EOF,
};

struct Event {
    enum EventType type;
    union {
        struct EventSymbolAdded    symbol_added;
        struct EventSymbolRemoved  symbol_removed;
        struct EventKeyUndo        key_undo;
        struct EventKeyRedo        key_redo;
        struct EventKeyDelete      key_delete;
        struct EventKeyBackspace   key_backspace;
        struct EventKeyText        key_text;
        struct EventKeyNavigation  key_navigation;
        struct EventRequestGoUp    request_go_up;
        struct EventRequestGoDown  request_go_down;
        struct EventRequestGoToBof request_go_to_bof;
        struct EventRequestGoToEof request_go_to_eof;
    } body;
};

struct Event *event_create_empty(void) {
    struct Event *event = malloc(sizeof(struct Event));
    return event;
}

struct Event *event_create_symbol_added(size_t index, char symbol) {
    struct Event *event = event_create_empty();
    event->type = EVENT_SYMBOL_ADDED;
    event->body.symbol_added = (struct EventSymbolAdded){
        .index = index,
        .symbol = symbol,
    };
    return event;
}

struct Event *event_create_symbol_removed(size_t index, char symbol) {
    struct Event *event = event_create_empty();
    event->type = EVENT_SYMBOL_REMOVED;
    event->body.symbol_removed = (struct EventSymbolRemoved){
        .index = index,
        .symbol = symbol,
    };
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

struct Event *event_create_request_go_up(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_REQUEST_GO_UP;
    event->body.request_go_up = (struct EventRequestGoUp){};
    return event;
}

struct Event *event_create_request_go_down(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_REQUEST_GO_DOWN;
    event->body.request_go_down = (struct EventRequestGoDown){};
    return event;
}

struct Event *event_create_request_go_to_bof(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_REQUEST_GO_TO_BOF;
    event->body.request_go_to_bof = (struct EventRequestGoToBof){};
    return event;
}

struct Event *event_create_request_go_to_eof(void) {
    struct Event *event = event_create_empty();
    event->type = EVENT_REQUEST_GO_TO_EOF;
    event->body.request_go_to_eof = (struct EventRequestGoToEof){};
    return event;
}

void event_destroy(struct Event *event) {
    free(event);
}

#define EVENT_CASE(NAME, name)                                  \
    case EVENT_##NAME: {                                        \
        event_handler->handle_##name(ctx, &(event->body.name)); \
        break;                                                  \
    };
void event_handle(const struct EventHandler *event_handler, struct Context *ctx, struct Event *event) {
    switch (event->type) {
        EVENT_CASE(SYMBOL_ADDED, symbol_added);
        EVENT_CASE(SYMBOL_REMOVED, symbol_removed);
        EVENT_CASE(KEY_UNDO, key_undo);
        EVENT_CASE(KEY_REDO, key_redo);
        EVENT_CASE(KEY_DELETE, key_delete);
        EVENT_CASE(KEY_BACKSPACE, key_backspace);
        EVENT_CASE(KEY_TEXT, key_text);
        EVENT_CASE(KEY_NAVIGATION, key_navigation);
        EVENT_CASE(REQUEST_GO_UP, request_go_up);
        EVENT_CASE(REQUEST_GO_DOWN, request_go_down);
        EVENT_CASE(REQUEST_GO_TO_BOF, request_go_to_bof);
        EVENT_CASE(REQUEST_GO_TO_EOF, request_go_to_eof);
        default: {
            panic("runtime error: unexpected command type while executing command");
        };
    }
}
#undef EVENT_CASE
