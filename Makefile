CFLAGS         := -Wall -Wextra -fPIC -g
CFLAGS_DEBUG   := -Wall -Wextra -fPIC -g
INCLUDES       := -Isrc
TUI_LIBS       := -lncurses

ENTRYPOINT_SRC := src/main.c
ENTRYPOINT_OBJ := src/main.o

NAME           := ste
TARGET         := $(NAME)

TUI_SRCS       := $(wildcard src/tui/*.c src/tui/**/*.c)
TUI_OBJS       := $(TUI_SRCS:.c=.o)

SRCS           := $(wildcard src/*.c src/**/*.c)
SRCS           := $(filter-out $(ENTRYPOINT_SRC), $(SRCS))
SRCS           := $(filter-out $(TUI_SRCS), $(SRCS))
OBJS           := $(SRCS:.c=.o)

.PHONY: default
default: clean build

.PHONY: clean
clean:
	rm -f $(TARGET) *.o src/*.o src/**/*.o playgrounds/*.o *.elf

.PHONY: build
build: $(TARGET)

.PHONY: playground-rope
playground-rope: $(OBJS) playgrounds/playground_rope.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_rope.elf $^

.PHONY: playground-gap-buffer
playground-gap-buffer: $(OBJS) playgrounds/playground_gap_buffer.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_gap_buffer.elf $^

.PHONY: playground-deque
playground-deque: $(OBJS) playgrounds/playground_deque.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_deque.elf $^

.PHONY: $(TARGET)
$(TARGET): $(OBJS) $(TUI_OBJS) $(ENTRYPOINT_OBJ)
	$(CC) $(INCLUDES) $(TUI_LIBS) $(CFLAGS) -o $(TARGET) $^

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<
