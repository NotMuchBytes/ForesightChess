cmake_minimum_required(VERSION 3.14)

project(ForesightChess)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

include_directories(include)

file(GLOB SOURCES
    src/*.cpp
)

add_executable(ForesightChess ${SOURCES})
target_link_libraries(ForesightChess PRIVATE raylib)
add_custom_command(TARGET ForesightChess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:ForesightChess>/assets)