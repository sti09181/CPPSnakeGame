##########################
##### CMakeLists.txt #####
##########################

cmake_minimum_required(VERSION 3.16)
project(CPPSnakeGame)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/Sources/*.c ${CMAKE_SOURCE_DIR}/Sources/*.cpp)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
