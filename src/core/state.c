#include "core/state.h"

#include <string.h>

const char *STATE_STR[] = {
    [STATE_START] = "START",
    [STATE_NORMAL] = "NORMAL",
    [STATE_INSERT] = "INSERT",
    [STATE_EXIT] = "EXIT",
};

const size_t STATE_STR_MAX_LENGTH = 6;
