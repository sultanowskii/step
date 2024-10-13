#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "collections/print.h"

struct EvictingDeque;

struct EvictingDeque *evicting_deque_create_emtpy();
struct EvictingDeque *evicting_deque_create(size_t max_size);
void                  evicting_deque_destroy(struct EvictingDeque *edeque, void (*destroy_value)(void *));

void *evicting_deque_push_front(struct EvictingDeque *edeque, void *value);
void *evicting_deque_pop_front(struct EvictingDeque *edeque);
void *evicting_deque_peek_front(struct EvictingDeque *edeque);
void *evicting_deque_push_back(struct EvictingDeque *edeque, void *value);
void *evicting_deque_pop_back(struct EvictingDeque *edeque);
void *evicting_deque_peek_back(struct EvictingDeque *edeque);
// Returns
void *evicting_deque_peek_back_offset(struct EvictingDeque *edeque, size_t offset);

size_t evicting_deque_get_size(const struct EvictingDeque *edeque);
size_t evicting_deque_get_max_size(const struct EvictingDeque *edeque);
size_t evicting_deque_is_size_limited(const struct EvictingDeque *edeque);

void evicting_deque_print(const struct EvictingDeque *edeque, print print_value);
void evicting_deque_print_with_indent(const struct EvictingDeque *edeque, print print_value, size_t indent_size);
void evicting_deque_debug_print(const struct EvictingDeque *edeque, print print_value);
void evicting_deque_debug_print_with_indent(const struct EvictingDeque *edeque, print print_value, size_t indent_size);
