#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "tui/core/_context.h"
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

struct EventGoUpRequest {
};

struct EventGoDownRequest {
};

struct EventHandler {
    void (*handle_symbol_added)(struct TuiContext *tctx, const struct EventSymbolAdded *symbol_added);
    void (*handle_symbol_removed)(struct TuiContext *tctx, const struct EventSymbolRemoved *symbol_removed);
    void (*handle_key_undo)(struct TuiContext *tctx, const struct EventKeyUndo *key_undo);
    void (*handle_key_redo)(struct TuiContext *tctx, const struct EventKeyRedo *key_redo);
    void (*handle_key_delete)(struct TuiContext *tctx, const struct EventKeyDelete *key_delete);
    void (*handle_key_backspace)(struct TuiContext *tctx, const struct EventKeyBackspace *key_backspace);
    void (*handle_key_text)(struct TuiContext *tctx, const struct EventKeyText *key_text);
    void (*handle_key_navigation)(struct TuiContext *tctx, const struct EventKeyNavigation *key_navigation);
    void (*handle_request_go_up)(struct TuiContext *tctx, const struct EventGoUpRequest *go_up_request);
    void (*handle_request_go_down)(struct TuiContext *tctx, const struct EventGoDownRequest *go_down_request);
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

void event_handle(const struct EventHandler *event_handler, struct TuiContext *tctx, struct Event *event);
