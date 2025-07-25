# Makefile for C++ Multiple Choice Exam Application
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = exam_app
BUILD_DIR = build
SRC_DIR = src

# Source files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/cauhoi.cpp \
          $(SRC_DIR)/diemthi.cpp \
          $(SRC_DIR)/lop.cpp \
          $(SRC_DIR)/monhoc.cpp \
          $(SRC_DIR)/sinhvien.cpp \
          $(SRC_DIR)/utility.cpp

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build target
$(BUILD_DIR)/$(TARGET): $(BUILD_DIR) $(OBJECTS)
	@echo "Linking..."
	$(CXX) $(OBJECTS) -o $@
	@echo "Build successful! Executable: $(BUILD_DIR)/$(TARGET)"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the application
run: $(BUILD_DIR)/$(TARGET)
	@echo "Running the application..."
	./$(BUILD_DIR)/$(TARGET)

# Build and run
dev: $(BUILD_DIR)/$(TARGET)
	@echo "Running the application..."
	./$(BUILD_DIR)/$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	@echo "Build files cleaned."

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(BUILD_DIR)/$(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all     - Build the application (default)"
	@echo "  run     - Build and run the application"
	@echo "  dev     - Build and run the application (alias for run)"
	@echo "  clean   - Remove build files"
	@echo "  debug   - Build with debug symbols"
	@echo "  help    - Show this help message"

.PHONY: all run dev clean debug help
