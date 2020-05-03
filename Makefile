ifeq ($(RELEASE), 1)
BUILD_TYPE_IDENTIFIER := release
else
BUILD_TYPE_IDENTIFIER := debug
endif

SOURCE_DIRECTORY := src
OBJECT_DIRECTORY := obj
BINARY_DIRECTORY := bin
BINARY_BASENAME = epitech-norm-helper

# Optimization options fed to the compiler
ifeq ($(RELEASE), 1)
OPTIMIZATION_FLAGS := -O3 -flto -g0 -s
else
OPTIMIZATION_FLAGS := -Og -ggdb3
endif

# Warning options fed to the compiler
WARNING_OPTIONS := -Wall -Wextra -Wpedantic

# Base command line when calling the compiler
# -std=c2x to enable C2x features
# -MMD -MP -MF $@.d to make the compiler generate dependency files
CFLAGS += $(OPTIMIZATION_FLAGS) $(WARNING_OPTIONS) -std=c2x -MMD
LDFLAGS += $(OPTIMIZATION_FLAGS) $(WARNING_OPTIONS)

# Main source file
SOURCE_FILES := main

OBJECT_FILES := $(addprefix $(OBJECT_DIRECTORY)/, $(addprefix $(BUILD_TYPE_IDENTIFIER)/, $(addsuffix .o, $(SOURCE_FILES))))
DEPENDENCY_FILES := $(addsuffix .d, $(OBJECT_FILES))

BINARY_FILE := $(BINARY_DIRECTORY)/$(BUILD_TYPE_IDENTIFIER)/$(BINARY_BASENAME)

# Default target
all: $(BINARY_FILE)

# Binary file target
$(BINARY_FILE): $(OBJECT_FILES)
	@mkdir -p $(@D)
	@echo "Linking to $@..."
	$(CC) $(LDFLAGS) $(OBJECT_FILES) -o $@
	@echo "Finished compiling $@"

# Generic source file target
$(OBJECT_DIRECTORY)/$(BUILD_TYPE_IDENTIFIER)/%.o: $(SOURCE_DIRECTORY)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Finished compiling $<"


# Include dependencies
include $(wildcard $(DEPENDENCY_FILES))

# Remove all object files and the binary
clean:
	@rm -rf $(OBJECT_DIRECTORY) $(BINARY_DIRECTORY)
