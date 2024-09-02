#pragma once

#include <stdbool.h>
#include <stddef.h>

struct EventQueue;

struct EventQueue *event_queue_create(void);
void               event_queue_destroy(struct EventQueue *event_queue);

void          event_queue_push_newline_added(struct EventQueue *event_queue, size_t prev_line_count);
void          event_queue_push_newline_removed(struct EventQueue *event_queue, size_t prev_line_count);
struct Event *event_queue_pop(struct EventQueue *event_queue);

bool event_queue_is_empty(const struct EventQueue *event_queue);
