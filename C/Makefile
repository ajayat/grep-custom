# Compiler options
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic -g

# Linker options
LDFLAGS := -lm -fsanitize=address,undefined

# Sources options
TARGET ?= mygrep.exe
BUILD_DIR ?= ./build
SOURCES ?= $(wildcard src/*.c)
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

# Colors options
GREEN = $(strip \033[0;32m)
DEFAULT = $(strip \033[0m)

# Commands
.PHONY: all run clean
all: $(TARGET) clean run

$(TARGET): $(OBJECTS)
	@echo "\n$(GREEN)Linking...$(DEFAULT)"
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Rule for building a C source file
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "\n$(GREEN)Compiling $<...$(DEFAULT)"
	$(CC) $(CFLAGS) -c $< -o $@

run:
	@echo "\n$(GREEN)Running $(TARGET):$(DEFAULT)"
	@./$(TARGET)

clean:
	@echo "\n$(GREEN)Cleaning...$(DEFAULT)"
	$(RM) -r $(BUILD_DIR)
