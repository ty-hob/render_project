# Directories
SRC_DIR := src
BUILD_DIR := build
BUILD_TEST_DIR := $(BUILD_DIR)/test
BIN_DIR := bin
INCLUDE_DIR := include

CC := clang
LIBS := $(shell pkg-config --libs sdl2 SDL2_gfx)
CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_gfx) \
		  -I$(INCLUDE_DIR) -O2 -Wall -Wextra -std=c99

# Source files
LIBS_SOURCES := $(wildcard src/*.c)
EXTERNAL_LIB_SOURCES := $(wildcard libs/*.c)
RENDENG_SOURCES := $(wildcard src/rendeng/*.c)
EDITOR_SOURCES := $(wildcard src/editor/*.c)
TEST_SOURCES := $(wildcard test/*.c)

# Object files
LIBS_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(LIBS_SOURCES))
EXTERNAL_LIB_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(EXTERNAL_LIB_SOURCES))
RENDENG_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(RENDENG_SOURCES)) \
				   $(LIBS_OBJECTS) \
				   $(EXTERNAL_LIB_OBJECTS)
EDITOR_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(EDITOR_SOURCES)) \
				  $(LIBS_OBJECTS)
TEST_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(TEST_SOURCES)) \
			 $(LIBS_OBJECTS)

# Executables
RENDENG_EXEC := $(BIN_DIR)/rendeng
EDITOR_EXEC := $(BIN_DIR)/editor
TEST_EXECS := $(patsubst %.c,$(BIN_DIR)/%,$(TEST_SOURCES))

all: $(RENDENG_EXEC) $(EDITOR_EXEC) $(TEST_EXECS)

tests: $(TEST_EXECS)

run_tests: $(TEST_EXECS)
	@for test in $(TEST_EXECS); do \
		echo "running $$test"; \
		$$test; \
	done

info:
	@echo "lib sources:\n  $(LIBS_SOURCES)"
	@echo "lib objects:\n  $(LIBS_OBJECTS)"
	@echo "external lib sources:\n  $(EXTERNAL_LIB_SOURCES)"
	@echo "external lib objects:\n  $(EXTERNAL_LIB_OBJECTS)"
	@echo "rendeng sources:\n  $(RENDENG_SOURCES)"
	@echo "rendeng objects:\n  $(RENDENG_OBJECTS)"
	@echo "editor sources:\n  $(EDITOR_SOURCES)"
	@echo "editor objects:\n  $(EDITOR_OBJECTS)"
	@echo "test sources:\n  $(TEST_SOURCES)"
	@echo "test objects:\n  $(TEST_OBJECTS)"
	@echo "test execs:\n  $(TEST_EXECS)"


# Building render engine object files and executable
$(RENDENG_EXEC): $(RENDENG_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(RENDENG_OBJECTS) $(LIBS) -o $@

# Building editor object files and executable
$(EDITOR_EXEC): $(EDITOR_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(EDITOR_OBJECTS) $(LIBS) -o $@

$(TEST_EXECS): $(TEST_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(TEST_OBJECTS) $(LIBS) -o $@

# Pattern rule for object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build and binary directories
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) render_result.ppm

.PHONY: all clean info tests
