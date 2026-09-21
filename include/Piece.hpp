#pragma once

#include <array>

struct Cell {
    int x;
    int y;
};

enum class Tetromino {
    I = 1,
    O,
    T,
    S,
    Z,
    J,
    L
};

class Piece {
public:
    Piece(Tetromino type = Tetromino::T, int rotation = 0, int x = 3, int y = -1);

    std::array<Cell, 4> cells() const;

    Tetromino type() const;
    int rotation() const;
    int x() const;
    int y() const;

    void move(int dx, int dy);
    void setPosition(int x, int y);
    Piece rotatedClockwise() const;

private:
    Tetromino type_;
    int rotation_;
    int x_;
    int y_;
};
