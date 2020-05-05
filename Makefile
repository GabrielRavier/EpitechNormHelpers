ifeq ($(RELEASE), 1)
BUILD_TYPE_IDENTIFIER := release
else
BUILD_TYPE_IDENTIFIER := debug
endif

SOURCE_DIRECTORY := src
OBJECT_DIRECTORY := obj
BINARY_DIRECTORY := bin
EXTERNAL_DIRECTORY := external
BINARY_BASENAME = epitech-norm-helper

# Optimization options fed to the compiler
ifeq ($(RELEASE), 1)
OPTIMIZATION_FLAGS := -O3 -flto -g0 -s
else
OPTIMIZATION_FLAGS := -O0 -ggdb3
endif

# Warning options fed to the compiler
WARNING_OPTIONS := -Wall -Wextra -Wpedantic

# Base command line when calling the compiler
# -std=c++20 to enable C++20 features
# -MMD -MP -MF $@.d to make the compiler generate dependency files
CXXFLAGS += $(OPTIMIZATION_FLAGS) $(WARNING_OPTIONS) -std=c++2a -MMD -I$(EXTERNAL_DIRECTORY)
LDFLAGS += $(OPTIMIZATION_FLAGS) $(WARNING_OPTIONS) -lfmt


# Main source file
SOURCE_FILES := main

# Option handling
SOURCE_FILES += options

# Actual program
SOURCE_FILES += program


OBJECT_FILES := $(addprefix $(OBJECT_DIRECTORY)/, $(addprefix $(BUILD_TYPE_IDENTIFIER)/, $(addsuffix .o, $(SOURCE_FILES))))
DEPENDENCY_FILES := $(OBJECT_FILES:.o=.d)

BINARY_FILE := $(BINARY_DIRECTORY)/$(BUILD_TYPE_IDENTIFIER)/$(BINARY_BASENAME)

# Default target
all: $(BINARY_FILE)

# Binary file target
$(BINARY_FILE): $(OBJECT_FILES)
	@mkdir -p $(@D)
	@echo "Linking to $@..."
	@$(CXX) $(LDFLAGS) $(OBJECT_FILES) -o $@
	@echo "Finished compiling $@"

# Generic source file target
$(OBJECT_DIRECTORY)/$(BUILD_TYPE_IDENTIFIER)/%.o: $(SOURCE_DIRECTORY)/%.cpp
	@mkdir -p $(@D)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Finished compiling $<"


# Include dependencies
include $(wildcard $(DEPENDENCY_FILES))

# Remove all object files and the binary
clean:
	@rm -rf $(OBJECT_DIRECTORY) $(BINARY_DIRECTORY)
