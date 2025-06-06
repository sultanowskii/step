#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "collections/print.h"
#include "nonstd/mem.h"

struct EvictingStack;

struct EvictingStack *evicting_stack_create(size_t max_size);
void                  evicting_stack_destroy(struct EvictingStack *estack, func_destroy destroy_value);

void *evicting_stack_push_back(struct EvictingStack *estack, void *value);
void *evicting_stack_pop_back(struct EvictingStack *estack);
void *evicting_stack_peek_back(struct EvictingStack *estack);
void *evicting_stack_peek_back_offset(struct EvictingStack *estack, size_t offset);

size_t evicting_stack_get_size(const struct EvictingStack *estack);
size_t evicting_stack_get_max_size(const struct EvictingStack *estack);
bool   evicting_stack_is_size_limited(const struct EvictingStack *estack);

void evicting_stack_print(const struct EvictingStack *estack, print print_value);
void evicting_stack_print_with_indent(const struct EvictingStack *estack, print print_value, size_t indent_size);
void evicting_stack_debug_print(const struct EvictingStack *estack, print print_value);
void evicting_stack_debug_print_with_indent(const struct EvictingStack *estack, print print_value, size_t indent_size);
