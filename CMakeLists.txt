cmake_minimum_required(VERSION 3.16)
project(TetrisSFML LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(tetris
    src/main.cpp
    src/Game.cpp
    src/Board.cpp
    src/Piece.cpp
)

target_include_directories(tetris PRIVATE include)
target_link_libraries(tetris PRIVATE sfml-graphics sfml-window sfml-system)

add_executable(tetris_core_tests
    tests.cpp
    src/Board.cpp
    src/Piece.cpp
)
target_include_directories(tetris_core_tests PRIVATE include)
