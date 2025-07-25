@echo off
echo ========================================
echo Building C++ Multiple Choice Exam App
echo ========================================

:: Create build directory if it doesn't exist
if not exist "build" mkdir build

:: Compile all source files
echo Compiling source files...
g++ -std=c++11 -Wall -Wextra -O2 ^
    src/main.cpp ^
    src/cauhoi.cpp ^
    src/diemthi.cpp ^
    src/lop.cpp ^
    src/monhoc.cpp ^
    src/sinhvien.cpp ^
    src/utility.cpp ^
    -o build/exam_app.exe

:: Check if compilation was successful
if %errorlevel% neq 0 (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.

:: Ask user if they want to run the application
set /p run_app="Do you want to run the application? (y/n): "
if /i "%run_app%"=="y" (
    echo.
    echo Running the application...
    echo ========================================
    build\exam_app.exe
) else (
    echo.
    echo Application built successfully. Run 'build\exam_app.exe' to start.
)

echo.
pause
