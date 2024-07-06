CFLAGS        = -Wall -Wextra -fPIC -g
CFLAGS_DEBUG  = -Wall -Wextra -fPIC -g
INCLUDES      = -Isrc -L.

NAME          = ste
SRCS          = $(wildcard src/*.c)
0BJS          = $(SRCS:.c=.o)
TARGET        = $(NAME).elf

.PHONY: default
default: clean build

.PHONY: clean
clean:
	rm -f $(TARGET) *.o src/*.o

.PHONY: build
build: $(TARGET)

$(TARGET): $(0BJS)
	$(CC) $(INCLUDES) $(CFLAGS) -o $(TARGET) $^

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<
