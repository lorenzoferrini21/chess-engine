cmake_minimum_required(VERSION 3.20)

project(Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(engine
    Engine.cpp
    board/board.cpp
    eval/evaluate.cpp
    move/makemove.cpp
    movegen/attacks.cpp
    movegen/movegen.cpp
    PERFT/perft.cpp
    search/search.cpp
    uci/uci.cpp
)

target_include_directories(engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/board
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/eval
    ${CMAKE_CURRENT_SOURCE_DIR}/move
    ${CMAKE_CURRENT_SOURCE_DIR}/movegen
    ${CMAKE_CURRENT_SOURCE_DIR}/PERFT
    ${CMAKE_CURRENT_SOURCE_DIR}/search
    ${CMAKE_CURRENT_SOURCE_DIR}/uci
)

# Keep build outputs contained in the build folder.
set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (MSVC)
  target_compile_options(engine PRIVATE /W3)
else()
  target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

