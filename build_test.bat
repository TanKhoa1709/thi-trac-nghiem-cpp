@echo off
echo Building test application...

g++ -std=c++11 -Iinclude ^
    main_test.cpp ^
    models/sinhvien.cpp ^
    models/diemthi.cpp ^
    models/lop.cpp ^
    models/monhoc.cpp ^
    models/cauhoi.cpp ^
    managers/quanlysinhvien.cpp ^
    managers/quanlydiem.cpp ^
    managers/quanlylop.cpp ^
    managers/quanlymonhoc.cpp ^
    managers/quanlycauhoi.cpp ^
    -o test_system.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run test_system.exe to start testing.
) else (
    echo Build failed! Check for compilation errors.
)

.\test_system.exe

pause