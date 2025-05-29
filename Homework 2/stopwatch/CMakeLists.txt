cmake_minimum_required(VERSION 3.5)

project(stopwatch VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        stopwatch.h
        stopwatch.cpp
)

add_executable(stopwatch ${PROJECT_SOURCES})

target_link_libraries(stopwatch PRIVATE
    Qt6::Widgets
)

install(TARGETS stopwatch
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

