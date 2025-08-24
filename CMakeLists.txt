cmake_minimum_required(VERSION 3.16)

project(ExamManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Include directories
include_directories(controllers)
include_directories(managers)
include_directories(models)
include_directories(utils)
include_directories(views)


set(PROJECT_SOURCES
    # Entry point
    main.cpp
    
    # Main window
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    
    # Controllers (will be added as we create them)
    controllers/AuthController.cpp
    controllers/AuthController.h
    
    # Views
    views/TeacherDashboard.cpp
    views/TeacherDashboard.h
    views/StudentDashboard.cpp
    views/StudentDashboard.h
    views/ExamWidget.cpp
    views/ExamWidget.h
    views/DetailedResultsWidget.cpp
    views/DetailedResultsWidget.h
    
    # Models (existing)
    models/cauhoi.cpp
    models/cauhoi.h
    models/diemthi.cpp
    models/diemthi.h
    models/lop.cpp
    models/lop.h
    models/monhoc.cpp
    models/monhoc.h
    models/sinhvien.cpp
    models/sinhvien.h
    
    # Managers (will be transformed to controllers)
    managers/quanlycauhoi.cpp
    managers/quanlycauhoi.h
    managers/quanlydiem.cpp
    managers/quanlydiem.h
    managers/quanlylop.cpp
    managers/quanlylop.h
    managers/quanlymonhoc.cpp
    managers/quanlymonhoc.h
    managers/quanlysinhvien.cpp
    managers/quanlysinhvien.h
    
    # Utils
    utils/BinaryTree.h
    utils/LinkedList.h
    utils/DynamicArray.h
    utils/InputValidator.cpp
    utils/InputValidator.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(testqt3
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET testqt3 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(testqt3 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(testqt3
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(testqt3 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.testqt3)
endif()
set_target_properties(testqt3 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS testqt3
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(testqt3)
endif()
