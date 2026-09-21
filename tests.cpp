#include "Board.hpp"
#include "Piece.hpp"

#include <cassert>
#include <iostream>

int main() {
    Board board;

    Piece piece(Tetromino::O, 0, 3, 0);
    assert(board.canPlace(piece));
    assert(board.lockPiece(piece));
    assert(!board.canPlace(piece));

    Board lineBoard;
    for (int x = 0; x < Board::Width; x += 2) {
        Piece domino(Tetromino::O, 0, x - 1, Board::Height - 2);
        // O cells land in columns x and x+1 for this position.
        assert(lineBoard.canPlace(domino));
        lineBoard.lockPiece(domino);
    }
    assert(lineBoard.clearFullRows() == 2);

    Piece leftWall(Tetromino::T, 0, -1, 5);
    assert(!board.canPlace(leftWall));

    Piece bottom(Tetromino::I, 0, 3, Board::Height - 1);
    assert(!board.canPlace(bottom));

    Piece rotating(Tetromino::L, 0, 3, 4);
    assert(rotating.rotatedClockwise().rotation() == 1);

    std::cout << "Core Tetris logic tests passed.\n";
    return 0;
}
