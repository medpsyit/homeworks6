cmake_minimum_required(VERSION 3.5)

project(HW1 VERSION 0.1 LANGUAGES CXX)

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
)

add_executable(HW1 ${PROJECT_SOURCES})

target_link_libraries(HW1 PRIVATE
    Qt6::Widgets
)

install(TARGETS HW1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
