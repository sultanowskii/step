#pragma once

// TODO: color
#define HIGHLIGHT_ON(win, y, x) mvwchgat(win, y, x, 1, A_REVERSE, 0, NULL)
#define HIGHLIGHT_OFF(win, y, x) mvwchgat(win, y, x, 1, A_NORMAL, 0, NULL)
