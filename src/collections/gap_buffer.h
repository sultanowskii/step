#pragma once

#include <stddef.h>

#define DEFAULT_GAP_SIZE 16

struct GapBuffer;

struct GapBuffer *gap_buffer_create_empty();
struct GapBuffer *gap_buffer_create_from_string(const char *s);

void gap_buffer_grow(struct GapBuffer *gb, size_t pos, size_t n);
void gap_buffer_move_gap(struct GapBuffer *gb, size_t pos);

void gap_buffer_insert(struct GapBuffer *gb, size_t pos, const char *s);
