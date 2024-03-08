cmake_minimum_required(VERSION 3.8)
project(budget-tracker-KUD0132)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

option(DISABLE_ASAN "Do not use Address sanitizer" OFF)

if(NOT DISABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

add_compile_options(-Wall -Wextra -pedantic)

include_directories(include)

# Set your source files
set(SOURCES src/main.cpp src/utils.cpp src/tracker.cpp)

# Use main.cpp to call and debug your functions
add_executable(main ${SOURCES})


target_link_libraries(main ncurses)