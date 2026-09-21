#include "Piece.hpp"

namespace {
using Shape = std::array<Cell, 4>;
using Rotations = std::array<Shape, 4>;

const std::array<Rotations, 7> SHAPES = {{
    // I
    {{{{{0,1},{1,1},{2,1},{3,1}}},
      {{{2,0},{2,1},{2,2},{2,3}}},
      {{{0,2},{1,2},{2,2},{3,2}}},
      {{{1,0},{1,1},{1,2},{1,3}}}}},
    // O
    {{{{{1,0},{2,0},{1,1},{2,1}}},
      {{{1,0},{2,0},{1,1},{2,1}}},
      {{{1,0},{2,0},{1,1},{2,1}}},
      {{{1,0},{2,0},{1,1},{2,1}}}}},
    // T
    {{{{{1,0},{0,1},{1,1},{2,1}}},
      {{{1,0},{1,1},{2,1},{1,2}}},
      {{{0,1},{1,1},{2,1},{1,2}}},
      {{{1,0},{0,1},{1,1},{1,2}}}}},
    // S
    {{{{{1,0},{2,0},{0,1},{1,1}}},
      {{{1,0},{1,1},{2,1},{2,2}}},
      {{{1,1},{2,1},{0,2},{1,2}}},
      {{{0,0},{0,1},{1,1},{1,2}}}}},
    // Z
    {{{{{0,0},{1,0},{1,1},{2,1}}},
      {{{2,0},{1,1},{2,1},{1,2}}},
      {{{0,1},{1,1},{1,2},{2,2}}},
      {{{1,0},{0,1},{1,1},{0,2}}}}},
    // J
    {{{{{0,0},{0,1},{1,1},{2,1}}},
      {{{1,0},{2,0},{1,1},{1,2}}},
      {{{0,1},{1,1},{2,1},{2,2}}},
      {{{1,0},{1,1},{0,2},{1,2}}}}},
    // L
    {{{{{2,0},{0,1},{1,1},{2,1}}},
      {{{1,0},{1,1},{1,2},{2,2}}},
      {{{0,1},{1,1},{2,1},{0,2}}},
      {{{0,0},{1,0},{1,1},{1,2}}}}}
}};
}

Piece::Piece(Tetromino type, int rotation, int x, int y)
    : type_(type), rotation_(rotation % 4), x_(x), y_(y) {}

std::array<Cell, 4> Piece::cells() const {
    const auto& shape = SHAPES[static_cast<int>(type_) - 1][rotation_];
    std::array<Cell, 4> result{};
    for (std::size_t i = 0; i < shape.size(); ++i) {
        result[i] = {shape[i].x + x_, shape[i].y + y_};
    }
    return result;
}

Tetromino Piece::type() const { return type_; }
int Piece::rotation() const { return rotation_; }
int Piece::x() const { return x_; }
int Piece::y() const { return y_; }

void Piece::move(int dx, int dy) {
    x_ += dx;
    y_ += dy;
}

void Piece::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

Piece Piece::rotatedClockwise() const {
    return Piece(type_, (rotation_ + 1) % 4, x_, y_);
}
