#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "collections/print.h"

struct Queue;

struct Queue *queue_create(void);
void          queue_destroy(struct Queue *queue, void (*destroy_value)(void *));

void *queue_pop(struct Queue *queue);
void *queue_peek(struct Queue *queue);
void  queue_push(struct Queue *queue, void *value);

size_t queue_get_size(const struct Queue *queue);

void queue_print(const struct Queue *queue, print print_value);
void queue_print_with_indent(const struct Queue *queue, print print_value, size_t indent_size);
void queue_debug_print(const struct Queue *queue, print print_value);
void queue_debug_print_with_indent(const struct Queue *queue, print print_value, size_t indent_size);
