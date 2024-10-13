#pragma once

#include <stddef.h>
#include <stdio.h>

struct GapBuffer;

struct GapBuffer *gap_buffer_create_from_string(const char *s);
void              gap_buffer_destroy(struct GapBuffer *gb);

void gap_buffer_insert(struct GapBuffer *gb, size_t index, const char *s);
void gap_buffer_insert_symbol(struct GapBuffer *gb, size_t index, char symbol);
void gap_buffer_delete(struct GapBuffer *gb, size_t index);
void gap_buffer_delete_n(struct GapBuffer *gb, size_t index, size_t n);

size_t gap_buffer_get_length(const struct GapBuffer *gb);

char gap_buffer_get_at(const struct GapBuffer *gb, size_t index);
void gap_buffer_set_at(struct GapBuffer *gb, size_t index, char c);

void gap_buffer_write_to_file(const struct GapBuffer *gb, FILE *f);

void gap_buffer_print(const struct GapBuffer *gb);
void gap_buffer_print_with_indent(const struct GapBuffer *gb, size_t indent_size);
void gap_buffer_debug_print(const struct GapBuffer *gb);
void gap_buffer_debug_print_with_indent(const struct GapBuffer *gb, size_t indent_size);
