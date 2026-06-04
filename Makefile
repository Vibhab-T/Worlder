# ─────────────────────────────────────────────
#  Project config
# ─────────────────────────────────────────────
PROJECT   := game
BUILD_DIR := build
SRC_DIR   := src
INC_DIR   := include
LIB_DIR   := lib

# ─────────────────────────────────────────────
#  Compiler / flags
# ─────────────────────────────────────────────
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I$(INC_DIR)
LDFLAGS  := -L$(LIB_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Release vs Debug
ifeq ($(MODE), debug)
    CXXFLAGS += -g -O0 -DDEBUG
else
    CXXFLAGS += -O2 -DNDEBUG
endif

# ─────────────────────────────────────────────
#  Sources → objects
# ─────────────────────────────────────────────
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# ─────────────────────────────────────────────
#  Targets
# ─────────────────────────────────────────────
.PHONY: all clean run

all: $(BUILD_DIR)/$(PROJECT)

# Link
$(BUILD_DIR)/$(PROJECT): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "✓ Built: $@"

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build dir if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(BUILD_DIR)/$(PROJECT)

clean:
	rm -rf $(BUILD_DIR)
	@echo "✓ Cleaned"
