# Sources options
TARGET ?= mygrep
BUILD_DIR ?= ./build
SOURCES ?= $(wildcard src/*.c) $(wildcard src/**/*.c)
INC_DIR ?= ./include

OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)
INC_FLAGS := $(addprefix -I,$(shell find $(INC_DIR) -type d))

# Compiler options
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic -g $(INC_FLAGS) -MMD -MP

# Linker options
LDFLAGS := -lm -fsanitize=address,undefined

# Colors options
GREEN = $(strip \033[0;32m)
DEFAULT = $(strip \033[0m)

# Commands
.PHONY: all clean
all: $(TARGET) clean run

$(TARGET): $(OBJECTS)
	@echo -e "\n$(GREEN)Linking...$(DEFAULT)"
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Rule for building a C source file
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "\n$(GREEN)Compiling $<...$(DEFAULT)"
	$(CC) $(CFLAGS) -c $< -o $@

run:
	@echo -e "\n$(GREEN)Running $(TARGET):$(DEFAULT)"
	@./$(TARGET)

clean:
	@echo -e "\n$(GREEN)Cleaning...$(DEFAULT)"
	$(RM) -r $(BUILD_DIR)
