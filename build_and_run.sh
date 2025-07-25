#!/bin/bash

echo "========================================"
echo "Building C++ Multiple Choice Exam App"
echo "========================================"

# Create build directory if it doesn't exist
mkdir -p build

# Compile all source files
echo "Compiling source files..."
g++ -std=c++11 -Wall -Wextra -O2 \
    src/main.cpp \
    src/cauhoi.cpp \
    src/diemthi.cpp \
    src/lop.cpp \
    src/monhoc.cpp \
    src/sinhvien.cpp \
    src/utility.cpp \
    -o build/exam_app

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo
    echo "========================================"
    echo "BUILD FAILED!"
    echo "========================================"
    exit 1
fi

echo
echo "========================================"
echo "BUILD SUCCESSFUL!"
echo "========================================"
echo

# Ask user if they want to run the application
read -p "Do you want to run the application? (y/n): " run_app
if [[ $run_app =~ ^[Yy]$ ]]; then
    echo
    echo "Running the application..."
    echo "========================================"
    ./build/exam_app
else
    echo
    echo "Application built successfully. Run './build/exam_app' to start."
fi
