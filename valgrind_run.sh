#!/bin/bash

make

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --leak-resolution=high \
         ./ste.elf
