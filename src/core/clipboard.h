#pragma once

#include <stddef.h>

struct Clipboard;

struct Clipboard *clipboard_create();
void              clipboard_destroy(struct Clipboard *clipboard);

const char *clipboard_get(struct Clipboard *clipboard);
void        clipboard_set(struct Clipboard *clipboard, const char *s, size_t length);
