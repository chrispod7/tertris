#pragma once

#include "Piece.hpp"
#include <array>

class Board {
public:
    static constexpr int Width = 10;
    static constexpr int Height = 20;

    Board();

    bool canPlace(const Piece& piece) const;
    bool lockPiece(const Piece& piece);
    int clearFullRows();
    int at(int x, int y) const;
    void reset();

private:
    std::array<std::array<int, Width>, Height> grid_{};
};
