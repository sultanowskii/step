CFLAGS         := -Wall -Wextra -fPIC -g
CFLAGS_DEBUG   := -Wall -Wextra -fPIC -g
INCLUDES       := -Isrc

ENTRYPOINT_SRC := src/main.c
ENTRYPOINT_OBJ := src/main.o

NAME           := ste
SRCS           := $(wildcard src/*.c src/**/*.c)
SRCS           := $(filter-out $(ENTRYPOINT_SRC), $(SRCS))
OBJS           := $(SRCS:.c=.o)
TARGET         := $(NAME)

.PHONY: default
default: clean build

.PHONY: clean
clean:
	rm -f $(TARGET) *.o src/*.o src/**/*.o playgrounds/*.o

.PHONY: build
build: $(TARGET)

.PHONY: playground-rope
playground-rope: $(OBJS) playgrounds/playground_rope.o
	echo $^
	$(CC) $(INCLUDES) $(CFLAGS) -o playground_rope.elf $^

.PHONY: $(TARGET)
$(TARGET): $(OBJS) $(ENTRYPOINT_OBJ)
	$(CC) $(INCLUDES) $(CFLAGS) -o $(TARGET) $^

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<
