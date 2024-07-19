#pragma once

#include <stddef.h>

struct Deque;

struct Deque *deque_create_emtpy();
struct Deque *deque_create();
void          deque_destroy(struct Deque *deque, void (*destroy_value)(void *));

void  deque_push_front(struct Deque *deque, void *value);
void *deque_pop_front(struct Deque *deque);

void  deque_push_back(struct Deque *deque, void *value);
void *deque_pop_back(struct Deque *deque);

size_t deque_get_size(const struct Deque *deque);

void deque_print(const struct Deque *deque, void (*print_value)(void *));
void deque_debug_print(const struct Deque *deque, void (*print_value)(void *));
