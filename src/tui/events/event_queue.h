#pragma once

#include <stdbool.h>
#include <stddef.h>

struct EventQueue;

struct EventQueue *event_queue_create(void);
void               event_queue_destroy(struct EventQueue *event_queue);

void          event_queue_push_symbol_added(struct EventQueue *event_queue, size_t index, char symbol);
void          event_queue_push_symbol_removed(struct EventQueue *event_queue, size_t index, char symbol);
void          event_queue_push_key_undo(struct EventQueue *event_queue);
void          event_queue_push_key_redo(struct EventQueue *event_queue);
void          event_queue_push_key_delete(struct EventQueue *event_queue);
void          event_queue_push_key_backspace(struct EventQueue *event_queue);
void          event_queue_push_key_text(struct EventQueue *event_queue, int key);
void          event_queue_push_key_copy(struct EventQueue *event_queue, size_t index, size_t length);
void          event_queue_push_key_cut(struct EventQueue *event_queue, size_t index, size_t length);
void          event_queue_push_key_paste(struct EventQueue *event_queue, size_t index);
void          event_queue_push_request_delete_string(struct EventQueue *event_queue, size_t index, size_t length);
void          event_queue_push_key_navigation(struct EventQueue *event_queue, int key);
void          event_queue_push_request_go_up(struct EventQueue *event_queue);
void          event_queue_push_request_go_down(struct EventQueue *event_queue);
void          event_queue_push_request_go_to_bof(struct EventQueue *event_queue);
void          event_queue_push_request_go_to_eof(struct EventQueue *event_queue);
struct Event *event_queue_pop(struct EventQueue *event_queue);

bool event_queue_is_empty(const struct EventQueue *event_queue);
