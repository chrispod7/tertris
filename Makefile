CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system

SOURCES := src/main.cpp src/Game.cpp src/Board.cpp src/Piece.cpp

.PHONY: all test clean

all: tetris

tetris: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@ $(SFML_LIBS)

test: tests.cpp src/Board.cpp src/Piece.cpp
	$(CXX) $(CXXFLAGS) tests.cpp src/Board.cpp src/Piece.cpp -o tetris_core_tests
	./tetris_core_tests

clean:
	rm -f tetris tetris_core_tests
