#pragma once

#include <stddef.h>

void print_prompt();
void print_prompt_with_message(const char *msg);

size_t read_size_t();
char  *read_str();
char   read_char();
