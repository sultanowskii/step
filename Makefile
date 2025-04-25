CFLAGS           := -Wall -Wswitch-enum -Wextra -fPIC -g
INCLUDES         := -Isrc
LIBS             := 

ENTRYPOINT_SRC   := src/main.c
ENTRYPOINT_OBJ   := src/main.o

NAME             := step
TARGET           := $(NAME)

TUI_SRCS         := $(shell find src/tui/ -type f -name '*.c')
TUI_OBJS         := $(TUI_SRCS:.c=.o)
TUI_LIBS         := -lpanel -lncurses

PGROUND_SRCS     := $(wildcard playgrounds/playground/*.c)
PGROUND_OBJS     := $(PGROUND_SRCS:.c=.o)

SRCS             := $(shell find src/ -type f -name '*.c')
SRCS             := $(filter-out $(ENTRYPOINT_SRC), $(SRCS))
SRCS             := $(filter-out $(TUI_SRCS), $(SRCS))
OBJS             := $(SRCS:.c=.o)

# good ol' make wildcard doesnt support nested search aka double star
TO_CLEANUP       := $(TARGET) $(shell find . -type f -name '*.o' -o -name '*.elf')

.PHONY: default
default: build

.PHONY: clean
clean:
	rm -f $(TO_CLEANUP)

.PHONY: build
build: $(TARGET)

.PHONY: playground-gap-buffer
playground-gap-buffer: $(OBJS) $(PGROUND_OBJS) playgrounds/playground_gap_buffer.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_gap_buffer.elf $^

.PHONY: playground-queue
playground-queue: $(OBJS) $(PGROUND_OBJS) playgrounds/playground_queue.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_deque.elf $^

.PHONY: playground-evicting-deque
playground-evicting-deque: $(OBJS) $(PGROUND_OBJS) playgrounds/playground_evicting_deque.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_evicting_deque.elf $^

.PHONY: playground-evicting-stack
playground-evicting-stack: $(OBJS) $(PGROUND_OBJS) playgrounds/playground_evicting_stack.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_evicting_stack.elf $^

.PHONY: playground-ncurses
playground-ncurses: $(OBJS) $(PGROUND_OBJS) playgrounds/playground_ncurses.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_ncurses.elf $^ $(TUI_LIBS)

.PHONY: playgrounds
playgrounds: playground-gap-buffer playground-queue playground-evicting-stack playground-evicting-deque playground-ncurses

.PHONY: $(TARGET)
$(TARGET): $(OBJS) $(TUI_OBJS) $(ENTRYPOINT_OBJ)
	$(CC) $(INCLUDES) $(CFLAGS) -o $(TARGET) $^ $(TUI_LIBS)

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

.PHONY: install
install:
	install -m 755 $(TARGET) /usr/bin/
