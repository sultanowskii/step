#pragma once

#include <stdbool.h>
#include <stddef.h>

#define MAX_LEAF_LENGTH 6

struct Rope;

struct RopePair {
    struct Rope *first;
    struct Rope *second;
};

struct Rope *create_empty_rope();
void         rope_destroy(struct Rope *rope);
struct Rope *rope_create_from_string(const char *s);

struct Rope    *rope_merge(struct Rope *r1, struct Rope *r2);
struct RopePair rope_split(struct Rope *rope, size_t index);

struct Rope *rope_delete(struct Rope *rope, size_t l, size_t r);
struct Rope *rope_insert(struct Rope *root, size_t index, const char *s);

char rope_get_at(const struct Rope *root, size_t index);
char rope_set_at(const struct Rope *root, size_t index, char c);
bool rope_is_index_within(const struct Rope *rope, size_t index);

size_t rope_fill_buffer_from_index(struct Rope *rope, char buffer[], size_t start_index, size_t max_symbols, size_t max_lines);

struct Rope *rope_rebalance(struct Rope *rope);

void rope_print(const struct Rope *rope);
void rope_debug_print(const struct Rope *rope);
