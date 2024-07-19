#include "collections/rope.h"

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "fmt.h"
#include "math.h"
#include "mem.h"
#include "str.h"

struct Rope {
    struct Rope *left;
    struct Rope *right;
    struct Rope *parent;
    char        *str;
    size_t       length;

    // for balancing
    size_t total_length;
    int    height;
};

struct Rope *_rope_create_from_string(const char *s, size_t l, size_t r);
void         _rope_debug_print(const struct Rope *rope, size_t depth);
char         _rope_get_at(const struct Rope *rope, size_t index);
void         _rope_set_at(const struct Rope *rope, size_t index, char c);

struct Rope *create_empty_rope() {
    struct Rope *rope = malloc(sizeof(struct Rope));
    rope->left = NULL;
    rope->right = NULL;
    rope->parent = NULL;
    rope->length = 0;
    rope->str = NULL;
    rope->total_length = 0;
    rope->height = 1;
    return rope;
}

void rope_destroy(struct Rope *rope) {
    if (rope == NULL) {
        return;
    }

    rope_destroy(rope->left);
    rope->left = FREED_DUMMY;
    rope_destroy(rope->right);
    rope->right = FREED_DUMMY;

    free(rope->str);
    rope->str = FREED_DUMMY;
    free(rope);
}

struct Rope *rope_create_from_string(const char *s) {
    return _rope_create_from_string(s, 0, strlen(s));
}

struct Rope *_rope_create_from_string(const char *s, size_t l, size_t r) {
    size_t n = r - l;

    struct Rope *root = create_empty_rope();
    if (n == 0) {
        return root;
    }

    root->length = n;
    root->total_length = n;

    if (n > MAX_LEAF_LENGTH) {
        size_t left_l = l;
        size_t left_r = l + n / 2;
        size_t right_l = l + n / 2;
        size_t right_r = r;

        struct Rope *left = _rope_create_from_string(s, left_l, left_r);
        left->parent = root;
        root->left = left;

        struct Rope *right = _rope_create_from_string(s, right_l, right_r);
        right->parent = root;
        root->right = right;
    } else {
        root->str = str_slice(s, l, r);
    }

    return root;
}

void rope_print(const struct Rope *rope) {
    rope_print_with_indent(rope, 0);
}

void _rope_print_with_indent(const struct Rope *rope) {
    if (rope->str != NULL) {
        printf("%s", rope->str);
        return;
    }

    if (rope->left != NULL) {
        _rope_print_with_indent(rope->left);
    }
    if (rope->right != NULL) {
        _rope_print_with_indent(rope->right);
    }
}

void rope_print_with_indent(const struct Rope *rope, size_t indent_size) {
    print_indent(indent_size);
    _rope_print_with_indent(rope);
}

void rope_debug_print(const struct Rope *rope) {
    rope_debug_print_with_indent(rope, 0);
}

void rope_debug_print_with_indent(const struct Rope *rope, size_t indent_size) {
    if (rope == NULL) {
        iprintf(indent_size, "NULL\n");
        return;
    }

    iprintf(indent_size, "Rope (0x%lx)\n", (size_t)rope);
    if (rope->str != NULL) {
        iprintf(indent_size, " str: %s\n", rope->str);
    } else {
        iprintf(indent_size, " str: (nil)\n");
    }

    iprintf(indent_size, " length: %zu\n", rope->length);

    iprintf(indent_size, " left:\n");
    rope_debug_print_with_indent(rope->left, indent_size + 2);

    iprintf(indent_size, " right:\n");
    rope_debug_print_with_indent(rope->right, indent_size + 2);
}

struct Rope *rope_merge(struct Rope *r1, struct Rope *r2) {
    struct Rope *root = create_empty_rope();

    root->left = r1;
    root->length += r1->length;
    r1->parent = root;

    root->right = r2;
    root->length += r2->length;
    r2->parent = root;

    return rope_rebalance(root);
}

// Index can't be 0 and length-1 - none of these make sense
struct RopePair rope_split(struct Rope *rope, size_t index) {
    struct Rope *left = NULL;
    struct Rope *right = NULL;

    if (rope->str != NULL) {
        left = _rope_create_from_string(rope->str, 0, index);
        right = _rope_create_from_string(rope->str, index, rope->length);
        goto RETURN;
    }

    if (index <= rope->left->length) {
        struct RopePair pair = rope_split(rope->left, index);
        left = rope_rebalance(pair.first);
        right = rope_merge(pair.second, rope->right);
    } else {
        struct RopePair pair = rope_split(rope->right, index - rope->left->length);
        left = rope_merge(rope->left, pair.first);
        right = rope_rebalance(pair.second);
    }

RETURN:
    // invalidate and clean the parent
    rope->left = NULL;
    rope->right = NULL;
    rope_destroy(rope);
    return (struct RopePair){
        left,
        right,
    };
}

struct Rope *rope_delete(struct Rope *rope, size_t l, size_t r) {
    if (rope == NULL) {
        return NULL;
    }

    if (l == 0) {
        struct RopePair pair = rope_split(rope, r);
        rope_destroy(pair.first);
        return pair.second;
    }

    if (r == rope->length - 1) {
        struct RopePair pair = rope_split(rope, l);
        rope_destroy(pair.second);
        return pair.first;
    }

    struct RopePair pair1 = rope_split(rope, l);
    struct Rope    *result_left = pair1.first;

    struct RopePair pair2 = rope_split(pair1.second, r - l);
    struct Rope    *result_right = pair2.second;

    rope_destroy(pair2.first);
    pair2.first = FREED_DUMMY;

    return rope_merge(result_left, result_right);
}

struct Rope *rope_insert(struct Rope *root, size_t index, const char *s) {
    struct Rope *piece = rope_create_from_string(s);

    if (index == 0) {
        return rope_merge(piece, root);
    }

    if (index == root->length - 1) {
        return rope_merge(root, piece);
    }

    struct RopePair pair = rope_split(root, index);
    struct Rope    *left_part = rope_merge(pair.first, piece);
    return rope_merge(left_part, pair.second);
}

char rope_get_at(const struct Rope *root, size_t index) {
    return _rope_get_at(root, index);
}

char _rope_get_at(const struct Rope *rope, size_t index) {
    if (rope->str != NULL) {
        return rope->str[index];
    }

    size_t left_length = rope->left->length;
    if (index < left_length) {
        return _rope_get_at(rope->left, index);
    } else {
        return _rope_get_at(rope->right, index - left_length);
    }
}

void rope_set_at(const struct Rope *root, size_t index, char c) {
    return _rope_set_at(root, index, c);
}

void _rope_set_at(const struct Rope *rope, size_t index, char c) {
    if (rope->str != NULL) {
        rope->str[index] = c;
        return;
    }

    size_t left_length = rope->left->length;
    if (index < left_length) {
        _rope_set_at(rope->left, index, c);
    } else {
        _rope_set_at(rope->right, index - left_length, c);
    }
}

bool rope_is_index_within(const struct Rope *rope, size_t index) {
    return index < rope->length;
}

// ==== DARK MAGIC BEGINS HERE ====
size_t _get_total_length(const struct Rope *rope) {
    if (rope == NULL) {
        return 0;
    }
    return rope->total_length;
}

int _get_height(const struct Rope *rope) {
    if (rope == NULL) {
        return 0;
    }
    return rope->height;
}

int _get_balance_factor(const struct Rope *rope) {
    if (rope == NULL) {
        return 0;
    }
    return _get_height(rope->left) - _get_height(rope->right);
}

void _recalculate_height(struct Rope *rope) {
    if (rope != NULL) {
        size_t left_height = _get_height(rope->left);
        size_t right_height = _get_height(rope->right);
        rope->height = 1 + MAX(left_height, right_height);
    }
}

void _recalculate_total_length(struct Rope *rope) {
    if (rope != NULL) {
        rope->total_length = rope->length + _get_total_length(rope->left) + _get_total_length(rope->right);
    }
}

void _recalculate_balancing_parameters(struct Rope *rope) {
    _recalculate_height(rope);
    _recalculate_total_length(rope);
}

struct Rope *_rope_rotate_left(struct Rope *rope) {
    struct Rope *new_root = rope->right;
    rope->right = new_root->left;
    if (new_root->left != NULL) {
        new_root->left->parent = rope;
    }
    new_root->parent = rope->parent;
    if (rope->parent == NULL) {
        // This is the root
    } else if (rope == rope->parent->left) {
        rope->parent->left = new_root;
    } else {
        rope->parent->right = new_root;
    }
    new_root->left = rope;
    rope->parent = new_root;

    _recalculate_balancing_parameters(rope);
    _recalculate_balancing_parameters(new_root);
    return new_root;
}

struct Rope *_rope_rotate_right(struct Rope *rope) {
    struct Rope *new_root = rope->left;
    rope->left = new_root->right;
    if (new_root->right != NULL) {
        new_root->right->parent = rope;
    }
    new_root->parent = rope->parent;
    if (rope->parent == NULL) {
        // This is the root
    } else if (rope == rope->parent->left) {
        rope->parent->left = new_root;
    } else {
        rope->parent->right = new_root;
    }
    new_root->right = rope;
    rope->parent = new_root;

    _recalculate_balancing_parameters(rope);
    _recalculate_balancing_parameters(new_root);
    return new_root;
}

struct Rope *rope_rebalance(struct Rope *rope) {
    if (rope == NULL) {
        return rope;
    }
    _recalculate_balancing_parameters(rope);

    int balance_factor = _get_balance_factor(rope);

    if (balance_factor > 1) {
        if (_get_balance_factor(rope->left) < 0) {
            rope->left = _rope_rotate_left(rope->left);
        }
        rope = _rope_rotate_right(rope);
    } else if (balance_factor < -1) {
        if (_get_balance_factor(rope->right) > 0) {
            rope->right = _rope_rotate_right(rope->right);
        }
        rope = _rope_rotate_left(rope);
    }
    return rope;
}
// ==== DARK MAGIC ENDS HERE ====

struct reader {
    size_t start_index;
    char  *buffer;
    size_t max_symbols;
    size_t max_lines;
    size_t symbols_written;
};
void _rope_fill_buffer(struct Rope *rope, struct reader *r);

size_t rope_fill_buffer_from_index(struct Rope *rope, char buffer[], size_t start_index, size_t max_symbols, size_t max_lines) {
    struct reader r = (struct reader){
        start_index,
        buffer,
        max_symbols,
        max_lines,
        0,
    };
    _rope_fill_buffer(rope, &r);

    return r.symbols_written;
}

void _str_copy(char *src, struct reader *r) {
    while (r->max_symbols > 0 && r->max_lines > 0 && *src != '\0') {
        *(r->buffer) = *src;
        (r->max_symbols)--;
        if (*src == '\n') {
            (r->max_lines)--;
        }
        (r->buffer)++;
        src++;
        r->symbols_written++;
    }
}

void _rope_fill_buffer(struct Rope *rope, struct reader *r) {
    if (rope == NULL) {
        return;
    }

    if (r->max_lines == 0 || r->max_symbols == 0) {
        return;
    }

    if (rope->str != NULL) {
        size_t len = strlen(rope->str);
        if (r->start_index < len) {
            _str_copy(rope->str + r->start_index, r);
            r->start_index = 0;
        } else {
            r->start_index -= len;
        }
        return;
    }

    _rope_fill_buffer(rope->left, r);
    _rope_fill_buffer(rope->right, r);
}
