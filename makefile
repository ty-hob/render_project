# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE_DIR := include

CC := clang
LIBS := $(shell pkg-config --libs sdl2 SDL2_gfx)
CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_gfx) -I$(INCLUDE_DIR) -O2 -Wall -Wextra -std=c99

# Source files
RENDENG_SRCS := $(wildcard src/rendeng/*.c)
EDITOR_SRCS := $(wildcard src/editor/*.c)
LIBS_SRCS := $(wildcard src/*.c)

# Object files
LIBS_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(LIBS_SRCS))
RENDENG_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(RENDENG_SRCS)) $(LIBS_OBJS)
EDITOR_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(EDITOR_SRCS)) $(LIBS_OBJS)

# Executables
RENDENG_EXEC := $(BIN_DIR)/rendeng
EDITOR_EXEC := $(BIN_DIR)/editor

all: $(RENDENG_EXEC) $(EDITOR_EXEC)

test:
	echo $(LIBS_SRCS)
	echo $(RENDENG_SRCS)
	echo $(EDITOR_SRCS)
	echo $(LIBS_OBJS)
	echo $(RENDENG_OBJS)
	echo $(EDITOR_OBJS)

# Building render engine object files and executable
$(RENDENG_EXEC): $(RENDENG_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(RENDENG_OBJS) $(LIBS) -o $@

# Building editor object files and executable
$(EDITOR_EXEC): $(EDITOR_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(EDITOR_OBJS) $(LIBS) -o $@

# Pattern rule for object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build and binary directories
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
