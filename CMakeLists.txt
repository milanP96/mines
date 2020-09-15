cmake_minimum_required(VERSION 3.17)
project(first_qt_app)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG
        PATHS /home/milan/Qt5.9.9/5.9.9/gcc_64/lib/cmake/Qt5Widgets
        NO_DEFAULT_PATH)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp widget.cpp widget.h field.cpp field.h game.cpp game.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui)