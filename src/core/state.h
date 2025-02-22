#pragma once

#include <stddef.h>

enum State {
    STATE_START,
    STATE_NORMAL,
    STATE_INSERT,
    STATE_VISUAL,
    STATE_EXIT,
};

extern const char  *STATE_STR[];
extern const size_t STATE_STR_MAX_LENGTH;
