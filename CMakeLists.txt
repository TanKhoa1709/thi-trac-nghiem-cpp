cmake_minimum_required(VERSION 3.16)

project(testqt2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core PrintSupport)

qt_standard_project_setup(REQUIRES 6.8)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/models
    ${CMAKE_CURRENT_SOURCE_DIR}/include/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
)

# Source files
set(SOURCES
    main.cpp
    src/core/appmanager.cpp
    src/core/authentication.cpp
    src/models/monhoc.cpp
    src/models/cauhoi.cpp
    src/models/lop.cpp
    src/models/sinhvien.cpp
    src/models/diemthi.cpp
    src/models/quanlythi.cpp
    src/controllers/examcontroller.cpp
    src/controllers/datacontroller.cpp
    src/ui/mainwindow.cpp
    src/ui/loginwidget.cpp
    src/ui/dashboardwidget.cpp
    src/ui/subjectmanagementwidget.cpp
    src/ui/questionmanagementwidget.cpp
    src/ui/studentmanagementwidget.cpp
    src/ui/classmanagementwidget.cpp
    src/ui/examwidget.cpp
    src/ui/reportwidget.cpp
)

# Header files
set(HEADERS
    include/core/appmanager.h
    include/core/authentication.h
    include/models/monhoc.h
    include/models/cauhoi.h
    include/models/lop.h
    include/models/sinhvien.h
    include/models/diemthi.h
    include/models/quanlythi.h
    include/controllers/examcontroller.h
    include/controllers/datacontroller.h
    include/ui/mainwindow.h
    include/ui/loginwidget.h
    include/ui/dashboardwidget.h
    include/ui/subjectmanagementwidget.h
    include/ui/questionmanagementwidget.h
    include/ui/studentmanagementwidget.h
    include/ui/classmanagementwidget.h
    include/ui/examwidget.h
    include/ui/reportwidget.h
)

# Enable Qt MOC for headers with Q_OBJECT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(apptestqt2
    ${SOURCES}
    ${HEADERS}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptestqt2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptestqt2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptestqt2
    PRIVATE Qt6::Widgets Qt6::Core Qt6::PrintSupport
)

include(GNUInstallDirs)
install(TARGETS apptestqt2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
