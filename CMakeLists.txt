cmake_minimum_required(VERSION 4.3.1)

project(roentgen VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets Concurrent)
find_package(ITK CONFIG REQUIRED COMPONENTS ITKIORAW)


add_subdirectory(gui)
add_subdirectory(src)
add_subdirectory(app)
