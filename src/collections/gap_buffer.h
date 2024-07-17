#pragma once

#include <stddef.h>

struct GapBuffer;

struct GapBuffer *gap_buffer_create_empty();
struct GapBuffer *gap_buffer_create_from_string(const char *s);
void              gap_buffer_destroy(struct GapBuffer *gb);

void gap_buffer_insert(struct GapBuffer *gb, size_t pos, const char *s);
void gap_buffer_delete(struct GapBuffer *gb, size_t pos);
void gap_buffer_delete_n(struct GapBuffer *gb, size_t pos, size_t n);

void gap_buffer_print(const struct GapBuffer *gb);
void gap_buffer_debug_print(const struct GapBuffer *gb);

char gap_buffer_get_at(const struct GapBuffer *gb, size_t pos);
void gap_buffer_set_at(struct GapBuffer *gb, size_t pos, char c);
