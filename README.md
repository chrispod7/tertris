# Tetris — C++ / SFML

A playable Tetris clone written in C++17 with SFML. The project separates game state into `Game`, `Board`, and `Piece` classes and uses a real-time event/update/render loop.

## Features

- Seven standard tetrominoes
- Real-time keyboard movement and rotation
- Soft drop and hard drop
- Collision detection against walls, floor, and placed blocks
- Full-row clearing
- Tetris-style scoring
- Automatic level progression and faster falling speed
- Next-piece preview
- Persistent high score saved to `highscore.txt`
- Restart after game over
- Simple wall-kick attempts when rotating near edges
- No external images or font assets required

## Controls

| Key | Action |
| --- | --- |
| Left / A | Move left |
| Right / D | Move right |
| Down / S | Soft drop |
| Up / W | Rotate clockwise |
| Space | Hard drop |
| R / Enter | Restart after game over |
| Escape | Quit |

Score, line count, level, high score, controls, and game-over status are displayed in the window title so the project does not depend on a font file.

## Project Structure

```text
include/
  Board.hpp
  Game.hpp
  Piece.hpp
src/
  Board.cpp
  Game.cpp
  Piece.cpp
  main.cpp
tests.cpp
CMakeLists.txt
Makefile
.gitignore
README.md
```

## Build with CMake

You need a C++17 compiler, CMake, and SFML 2.5+.

### Linux / Ubuntu / WSL

Install SFML:

```bash
sudo apt update
sudo apt install libsfml-dev cmake g++
```

Build:

```bash
mkdir build
cd build
cmake ..
cmake --build .
./tetris
```

### macOS

```bash
brew install sfml cmake
mkdir build && cd build
cmake ..
cmake --build .
./tetris
```

### Windows

Install SFML and configure CMake so `find_package(SFML ...)` can find your SFML installation. You can also use vcpkg:

```powershell
vcpkg install sfml
```

Then configure CMake using your vcpkg toolchain file.

## Logic Tests

The board and piece logic do not depend on SFML, so they can be tested separately:

```bash
make test
```

The tests check piece placement, collisions, rotation state, and row clearing.

## Design

### `Piece`
Stores tetromino type, rotation, and board position. Each tetromino has four predefined rotation states.

### `Board`
Owns the 10x20 playfield. It validates piece placement, locks pieces into the grid, and clears completed rows.

### `Game`
Owns the main SFML window and coordinates input, timing, movement, scoring, level progression, rendering, piece generation, next-piece preview, game over, and high-score persistence.

## Resume Description

**Tetris | C++, SFML**

- Developed a playable Tetris game in C++ and SFML using an object-oriented design (`Game`, `Board`, `Piece`) and a real-time game loop for input, movement, rotation, and drops.
- Implemented collision detection, full-row clearing, scoring, level speed-up, next-piece preview, and persistent high-score storage.

## Possible Extensions

- Seven-bag piece randomization
- Hold-piece system
- Ghost piece
- SRS wall kicks
- Sound effects and music
- In-window text using an SFML font
- Pause menu
