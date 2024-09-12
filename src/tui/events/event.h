#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "tui/core/_context.h"

struct Event;

struct EventNewlineAdded {
};

struct EventNewlineRemoved {
};

struct EventKeyUndo {
};

struct EventKeyRedo {
};

struct EventKeyDelete {
};

struct EventKeyBackspace {
};

struct EventHandler {
    void (*handle_newline_added)(struct TuiContext *tctx, const struct EventNewlineAdded *newline_added);
    void (*handle_newline_removed)(struct TuiContext *tctx, const struct EventNewlineRemoved *newline_removed);
    void (*handle_key_undo)(struct TuiContext *tctx, const struct EventKeyUndo *key_undo);
    void (*handle_key_redo)(struct TuiContext *tctx, const struct EventKeyRedo *key_redo);
    void (*handle_key_delete)(struct TuiContext *tctx, const struct EventKeyDelete *key_delete);
    void (*handle_key_backspace)(struct TuiContext *tctx, const struct EventKeyBackspace *key_backspace);
};

struct Event *event_create_newline_added(void);
struct Event *event_create_newline_removed(void);
struct Event *event_create_key_undo(void);
struct Event *event_create_key_redo(void);
struct Event *event_create_key_delete(void);
struct Event *event_create_key_backspace(void);
void          event_destroy(struct Event *event);

void event_handle(const struct EventHandler *event_handler, struct TuiContext *tctx, struct Event *event);
