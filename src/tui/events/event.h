#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "tui/core/_context.h"
#include "tui/navigation.h"

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

struct EventKeyText {
    int key;
};

struct EventKeyNavigation {
    int key;
};

struct EventSingularNavigationRequest {
    enum SingularNavigationRequest req;
};

struct EventHandler {
    void (*handle_newline_added)(struct TuiContext *tctx, const struct EventNewlineAdded *newline_added);
    void (*handle_newline_removed)(struct TuiContext *tctx, const struct EventNewlineRemoved *newline_removed);
    void (*handle_key_undo)(struct TuiContext *tctx, const struct EventKeyUndo *key_undo);
    void (*handle_key_redo)(struct TuiContext *tctx, const struct EventKeyRedo *key_redo);
    void (*handle_key_delete)(struct TuiContext *tctx, const struct EventKeyDelete *key_delete);
    void (*handle_key_backspace)(struct TuiContext *tctx, const struct EventKeyBackspace *key_backspace);
    void (*handle_key_text)(struct TuiContext *tctx, const struct EventKeyText *key_text);
    void (*handle_key_navigation)(struct TuiContext *tctx, const struct EventKeyNavigation *key_navigation);
    void (*handle_singular_navigation_request)(struct TuiContext *tctx, const struct EventSingularNavigationRequest *singular_navigation_request);
};

struct Event *event_create_newline_added(void);
struct Event *event_create_newline_removed(void);
struct Event *event_create_key_undo(void);
struct Event *event_create_key_redo(void);
struct Event *event_create_key_delete(void);
struct Event *event_create_key_backspace(void);
struct Event *event_create_key_text(int key);
struct Event *event_create_key_navigation(int key);
struct Event *event_create_singular_navigation_request(enum SingularNavigationRequest req);
void          event_destroy(struct Event *event);

void event_handle(const struct EventHandler *event_handler, struct TuiContext *tctx, struct Event *event);
