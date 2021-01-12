cmake_minimum_required(VERSION 3.17)
project(hexSerial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets SerialPort REQUIRED)

add_executable(${PROJECT_NAME}
        itDialog.cpp
        itDialog.h
        itDialog.ui
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        settingsdialog.cpp
        settingsdialog.h
        settingsdialog.ui
        icons.qrc
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::SerialPort
)
