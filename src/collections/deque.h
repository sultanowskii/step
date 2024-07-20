#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "collections/print.h"

struct Deque;

struct Deque *deque_create_emtpy();
struct Deque *deque_create();
void          deque_destroy(struct Deque *deque, void (*destroy_value)(void *));

void  deque_push_front(struct Deque *deque, void *value);
void *deque_pop_front(struct Deque *deque);
void  deque_push_back(struct Deque *deque, void *value);
void *deque_pop_back(struct Deque *deque);

size_t deque_get_size(const struct Deque *deque);
size_t deque_get_max_size(const struct Deque *deque);
bool   deque_is_size_limited(const struct Deque *deque);

void deque_print(const struct Deque *deque, void (*print_value)(void *));
void deque_print_with_indent(const struct Deque *deque, print print_value, size_t indent_size);
void deque_debug_print(const struct Deque *deque, void (*print_value)(void *));
void deque_debug_print_with_indent(const struct Deque *deque, print print_value, size_t indent_size);
