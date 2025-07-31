@echo off
echo Building test application...

g++ -std=c++11 -Iinclude ^
    main_test.cpp ^
    src/models/sinhvien.cpp ^
    src/models/diemthi.cpp ^
    src/models/lop.cpp ^
    src/models/monhoc.cpp ^
    src/models/cauhoi.cpp ^
    src/manager/quanlysinhvien.cpp ^
    src/manager/quanlydiem.cpp ^
    src/manager/quanlylop.cpp ^
    src/manager/quanlymonhoc.cpp ^
    src/manager/quanlycauhoi.cpp ^
    -o test_system.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run test_system.exe to start testing.
) else (
    echo Build failed! Check for compilation errors.
)

pause