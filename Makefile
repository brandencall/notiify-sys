CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iincludes

# Output folders
BIN_DIR := bin
OBJ_DIR := obj

# Sources
MONITOR_SRC := $(wildcard monitor/*.cpp)
NOTIFIER_SRC := $(wildcard notifier/*.cpp)
SHARED_SRC := $(wildcard shared/*.cpp)

# Object files
MONITOR_OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(MONITOR_SRC))
NOTIFIER_OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(NOTIFIER_SRC))
SHARED_OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SHARED_SRC))

# Targets
MONITOR_BIN := $(BIN_DIR)/monitor_app
NOTIFIER_BIN := $(BIN_DIR)/notifier_app

.PHONY: all clean dirs

all: dirs $(MONITOR_BIN) $(NOTIFIER_BIN)

# Create bin/ and obj/ if not exist
dirs:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)/monitor $(OBJ_DIR)/notifier $(OBJ_DIR)/shared

# Build monitor
$(MONITOR_BIN): $(MONITOR_OBJ) $(SHARED_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build notifier
$(NOTIFIER_BIN): $(NOTIFIER_OBJ) $(SHARED_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile each .cpp file to .o in obj/
$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
