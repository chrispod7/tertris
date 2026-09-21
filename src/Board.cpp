#include "Board.hpp"
#include <algorithm>

Board::Board() {
    reset();
}

bool Board::canPlace(const Piece& piece) const {
    for (const Cell& cell : piece.cells()) {
        if (cell.x < 0 || cell.x >= Width || cell.y >= Height) {
            return false;
        }
        if (cell.y >= 0 && grid_[cell.y][cell.x] != 0) {
            return false;
        }
    }
    return true;
}

bool Board::lockPiece(const Piece& piece) {
    bool fullyVisible = true;
    const int value = static_cast<int>(piece.type());

    for (const Cell& cell : piece.cells()) {
        if (cell.y < 0) {
            fullyVisible = false;
            continue;
        }
        if (cell.x >= 0 && cell.x < Width && cell.y < Height) {
            grid_[cell.y][cell.x] = value;
        }
    }
    return fullyVisible;
}

int Board::clearFullRows() {
    int writeRow = Height - 1;
    int cleared = 0;

    for (int readRow = Height - 1; readRow >= 0; --readRow) {
        const bool full = std::all_of(grid_[readRow].begin(), grid_[readRow].end(),
                                      [](int cell) { return cell != 0; });
        if (full) {
            ++cleared;
            continue;
        }
        if (writeRow != readRow) {
            grid_[writeRow] = grid_[readRow];
        }
        --writeRow;
    }

    while (writeRow >= 0) {
        grid_[writeRow].fill(0);
        --writeRow;
    }

    return cleared;
}

int Board::at(int x, int y) const {
    if (x < 0 || x >= Width || y < 0 || y >= Height) {
        return -1;
    }
    return grid_[y][x];
}

void Board::reset() {
    for (auto& row : grid_) {
        row.fill(0);
    }
}
