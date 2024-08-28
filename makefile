# Directories
SRC_DIR := src
BUILD_DIR := build
BUILD_TEST_DIR := $(BUILD_DIR)/test
BIN_DIR := bin
INCLUDE_DIR := include

CC := clang
LIBS := $(shell pkg-config --libs sdl2 SDL2_gfx)
CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_gfx) -I$(INCLUDE_DIR) -O2 -Wall -Wextra -std=c99

# Source files
RENDENG_SRCS := $(wildcard src/rendeng/*.c)
EDITOR_SRCS := $(wildcard src/editor/*.c)
LIBS_SRCS := $(wildcard src/*.c)
TEST_SRCS := $(wildcard test/*.c)

# Object files
LIBS_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(LIBS_SRCS))
RENDENG_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(RENDENG_SRCS)) $(LIBS_OBJS)
EDITOR_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(EDITOR_SRCS)) $(LIBS_OBJS)
TEST_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(TEST_SRCS)) $(LIBS_OBJS)

# Executables
RENDENG_EXEC := $(BIN_DIR)/rendeng
EDITOR_EXEC := $(BIN_DIR)/editor
TEST_EXECS := $(patsubst %.c,$(BIN_DIR)/%,$(TEST_SRCS))

all: $(RENDENG_EXEC) $(EDITOR_EXEC) $(TEST_EXECS)

tests: $(TEST_EXECS)

run_tests: $(TEST_EXECS)
	@for test in $(TEST_EXECS); do \
		echo "running $$test"; \
		$$test; \
	done

info:
	echo "lib sources: $(LIBS_SRCS)"
	echo "lib objects: $(LIBS_OBJS)"
	echo "rendeng sources: $(RENDENG_SRCS)"
	echo "rendeng objects: $(RENDENG_OBJS)"
	echo "editor sources: $(EDITOR_SRCS)"
	echo "editor objects: $(EDITOR_OBJS)"
	echo "test sources: $(TEST_SRCS)"
	echo "test objects: $(TEST_OBJS)"
	echo "test execs: $(TEST_EXECS)"

# Building render engine object files and executable
$(RENDENG_EXEC): $(RENDENG_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(RENDENG_OBJS) $(LIBS) -o $@

# Building editor object files and executable
$(EDITOR_EXEC): $(EDITOR_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(EDITOR_OBJS) $(LIBS) -o $@

$(TEST_EXECS): $(TEST_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(TEST_OBJS) $(LIBS) -o $@

# Pattern rule for object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build and binary directories
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) render_result.ppm

.PHONY: all clean info tests
