#pragma once

#include <stddef.h>

void print_prompt(void);
void print_prompt_with_message(const char *msg);

size_t read_size_t(void);
char  *read_str(void);
char   read_char(void);
