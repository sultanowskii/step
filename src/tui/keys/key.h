#pragma once

#include <ncurses.h>

#ifndef CTRL
#define CTRL(c) ((c) & 0x1F)
#endif

// Note that Alt+Key results in [0x1b, key] sequence.
#define ESC (0x1b)
