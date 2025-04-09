#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "core/_context.h"

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

struct EventKeyCopy {
    size_t index;
    size_t length;
};

struct EventKeyCut {
    size_t index;
    size_t length;
};

struct EventKeyPaste {
    size_t index;
};

struct EventKeyNavigation {
    int key;
};

struct EventRequestGoUp {
};

struct EventRequestGoDown {
};

struct EventRequestGoToBof {
};

struct EventRequestGoToEof {
};

#define DEFINE_HANDLER_FIELD(Name, name) \
    void (*handle_##name)(struct Context * ctx, const struct Event##Name *name);
struct EventHandler {
    DEFINE_HANDLER_FIELD(SymbolAdded, symbol_added)
    DEFINE_HANDLER_FIELD(SymbolRemoved, symbol_removed)
    DEFINE_HANDLER_FIELD(KeyUndo, key_undo)
    DEFINE_HANDLER_FIELD(KeyRedo, key_redo)
    DEFINE_HANDLER_FIELD(KeyDelete, key_delete)
    DEFINE_HANDLER_FIELD(KeyBackspace, key_backspace)
    DEFINE_HANDLER_FIELD(KeyText, key_text)
    DEFINE_HANDLER_FIELD(KeyCopy, key_copy)
    DEFINE_HANDLER_FIELD(KeyCut, key_cut)
    DEFINE_HANDLER_FIELD(KeyPaste, key_paste)
    DEFINE_HANDLER_FIELD(KeyNavigation, key_navigation)
    DEFINE_HANDLER_FIELD(RequestGoUp, request_go_up)
    DEFINE_HANDLER_FIELD(RequestGoDown, request_go_down)
    DEFINE_HANDLER_FIELD(RequestGoToBof, request_go_to_bof)
    DEFINE_HANDLER_FIELD(RequestGoToEof, request_go_to_eof)
};
#undef DEFINE_HANDLER_FIELD

#define DEFINE_EVENT_CREATE(name) struct Event *event_create_##name
DEFINE_EVENT_CREATE(symbol_added)(size_t index, char symbol);
DEFINE_EVENT_CREATE(symbol_removed)(size_t index, char symbol);
DEFINE_EVENT_CREATE(key_undo)(void);
DEFINE_EVENT_CREATE(key_redo)(void);
DEFINE_EVENT_CREATE(key_delete)(void);
DEFINE_EVENT_CREATE(key_backspace)(void);
DEFINE_EVENT_CREATE(key_text)(int key);
DEFINE_EVENT_CREATE(key_copy)(size_t index, size_t length);
DEFINE_EVENT_CREATE(key_cut)(size_t index, size_t length);
DEFINE_EVENT_CREATE(key_paste)(size_t index);
DEFINE_EVENT_CREATE(key_navigation)(int key);
DEFINE_EVENT_CREATE(request_go_up)(void);
DEFINE_EVENT_CREATE(request_go_down)(void);
DEFINE_EVENT_CREATE(request_go_to_bof)(void);
DEFINE_EVENT_CREATE(request_go_to_eof)(void);
#undef DEFINE_EVENT_CREATE

void event_destroy(struct Event *event);

void event_handle(const struct EventHandler *event_handler, struct Context *ctx, struct Event *event);
