#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "core/_context.h"
#include "tui/navigation.h"

struct Event;

struct EventSymbolAdded {
    size_t index;
    char   symbol;
};

struct EventSymbolRemoved {
    size_t index;
    char   symbol;
};

struct EventKeyUndo {
};

struct EventKeyRedo {
};

struct EventKeyDelete {
};

struct EventKeyBackspace {
};

struct EventKeyText {
    int key;
};

struct EventKeyNavigation {
    int key;
};

struct EventRequestGoUp {
};

struct EventRequestGoDown {
};

struct EventHandler {
    void (*handle_symbol_added)(struct Context *ctx, const struct EventSymbolAdded *symbol_added);
    void (*handle_symbol_removed)(struct Context *ctx, const struct EventSymbolRemoved *symbol_removed);
    void (*handle_key_undo)(struct Context *ctx, const struct EventKeyUndo *key_undo);
    void (*handle_key_redo)(struct Context *ctx, const struct EventKeyRedo *key_redo);
    void (*handle_key_delete)(struct Context *ctx, const struct EventKeyDelete *key_delete);
    void (*handle_key_backspace)(struct Context *ctx, const struct EventKeyBackspace *key_backspace);
    void (*handle_key_text)(struct Context *ctx, const struct EventKeyText *key_text);
    void (*handle_key_navigation)(struct Context *ctx, const struct EventKeyNavigation *key_navigation);
    void (*handle_request_go_up)(struct Context *ctx, const struct EventRequestGoUp *request_go_up);
    void (*handle_request_go_down)(struct Context *ctx, const struct EventRequestGoDown *request_go_down);
};

struct Event *event_create_symbol_added(size_t index, char symbol);
struct Event *event_create_symbol_removed(size_t index, char symbol);
struct Event *event_create_key_undo(void);
struct Event *event_create_key_redo(void);
struct Event *event_create_key_delete(void);
struct Event *event_create_key_backspace(void);
struct Event *event_create_key_text(int key);
struct Event *event_create_key_navigation(int key);
struct Event *event_create_request_go_up(void);
struct Event *event_create_request_go_down(void);
void          event_destroy(struct Event *event);

void event_handle(const struct EventHandler *event_handler, struct Context *ctx, struct Event *event);
