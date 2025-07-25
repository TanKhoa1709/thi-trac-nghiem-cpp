# Build Instructions for C++ Multiple Choice Exam Application

This document explains how to build and run the C++ Multiple Choice Exam Application.

## Prerequisites

- **C++ Compiler**: GCC (g++) with C++11 support or newer
- **Operating System**: Windows, Linux, or macOS

## Build Scripts Available

### 1. Windows (Batch Script)
```bash
.\build_and_run.bat
```
- Compiles all source files using g++
- Creates executable in `build/exam_app.exe`
- Prompts to run the application after successful build

### 2. Linux/macOS (Shell Script)
```bash
./build_and_run.sh
```
- Compiles all source files using g++
- Creates executable in `build/exam_app`
- Prompts to run the application after successful build

### 3. Cross-Platform (Makefile)
```bash
# Use the new comprehensive Makefile
cp Makefile.new Makefile

# Build only
make

# Build and run
make run
# or
make dev

# Clean build files
make clean

# Debug build
make debug

# Show help
make help
```

## Manual Build Command

If you prefer to build manually:

```bash
# Create build directory
mkdir build

# Compile all source files
g++ -std=c++11 -Wall -Wextra -O2 \
    src/main.cpp \
    src/cauhoi.cpp \
    src/diemthi.cpp \
    src/lop.cpp \
    src/monhoc.cpp \
    src/sinhvien.cpp \
    src/utility.cpp \
    -o build/exam_app
```

## Running the Application

After building, run the executable:

**Windows:**
```bash
build\exam_app.exe
```

**Linux/macOS:**
```bash
./build/exam_app
```

## Application Features

- Teacher login (admin/123456)
- Student login with database lookup
- Class and subject management
- Question management with BST data structure
- Randomized exam taking
- Score calculation and grade viewing

## Troubleshooting

1. **Compiler not found**: Ensure g++ is installed and in your PATH
2. **Permission denied**: On Linux/macOS, make sure the script is executable: `chmod +x build_and_run.sh`
3. **Build errors**: Check that all source files are present in the `src/` directory

## Notes

- The application creates a `build/` directory for compiled output
- All source files are located in the `src/` directory
- The build process includes compiler warnings for better code quality
