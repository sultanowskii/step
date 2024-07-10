#include "collections/gap_buffer.h"

#include <stddef.h>
#include <string.h>

struct GapBuffer {
    char  *buffer;
    size_t size;
    size_t length;
    size_t left;
    size_t right;
};

struct GapBuffer *gap_buffer_create_empty() {
    struct GapBuffer *gb = malloc(sizeof(struct GapBuffer));
    gb->buffer = NULL;
    gb->size = 0;
    gb->length = 0;
    gb->left = 0;
    gb->right = 0;
    return gb;
}

struct GapBuffer *gap_buffer_create_from_string(const char *s) {
    struct GapBuffer *gb = gap_buffer_create_empty();
    // TODO: implement
}

void gap_buffer_grow(struct GapBuffer *gb, size_t pos, size_t n) {
    size_t to_move = gb->length - pos;

    for (size_t i = gb->length + to_move; i > gb->length; i--) {
        gb->buffer[i] = gb->buffer[i - to_move];
    }

    for (size_t i = pos; i < pos + n; i++) {
        gb->buffer[i] = '_'; // TODO: remove?
    }

    gb->length += n;
    gb->left = pos;
    gb->right = gb->left + n;
}

void _gap_buffer_move_left(struct GapBuffer *gb, size_t pos) {
    while (gb->left > pos) {
        gb->left--;
        gb->right--;
        gb->buffer[gb->right + 1] = gb->buffer[gb->left];
        gb->buffer[gb->left] = '_'; // TODO: remove?
    }
}

void _gap_buffer_move_right(struct GapBuffer *gb, size_t pos) {
    while (gb->left < pos) {
        gb->left++;
        gb->right++;
        gb->buffer[gb->left - 1] = gb->buffer[gb->right];
        gb->buffer[gb->right] = '_'; // TODO: remove?
    }
}

void gap_buffer_move_gap(struct GapBuffer *gb, size_t pos) {
    if (pos < gb->left) {
        _gap_buffer_move_left(gb, pos);
    } else {
        _gap_buffer_move_right(gb, pos);
    }
}

void gap_buffer_insert(struct GapBuffer *gb, size_t pos, const char *s) {
    size_t n = strlen(s);

    if (n == 0) {
        return;
    }

    if (gb->left != pos) {
        gap_buffer_move_gap(gb, pos);
    }

    size_t i = 0;
    size_t current_pos = pos; // TODO: just use gb->left?

    while (i < n) {
        if (gb->left == gb->right) {
            gap_buffer_grow(gb, current_pos, DEFAULT_GAP_SIZE);
        }

        gb->buffer[gb->left] = s[i];
        gb->left++;
        i++;
        current_pos++;
    }
}

// TODO: delete()
// TODO: get_at
// TODO: set_at
// TODO: iterate
