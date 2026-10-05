#pragma once

#include "Board.hpp"
#include <SFML/Graphics.hpp>
#include <random>
#include <string>

class Game {
public:
    Game();
    void run();

private:
    static constexpr int CellSize = 30;
    static constexpr int BoardOffsetX = 40;
    static constexpr int BoardOffsetY = 40;

    sf::RenderWindow window_;
    Board board_;
    Piece current_;
    Piece next_;
    std::mt19937 rng_;

    sf::Clock fallClock_;
    int score_ = 0;
    int lines_ = 0;
    int level_ = 1;
    int highScore_ = 0;
    bool gameOver_ = false;

    Piece randomPiece();
    void handleEvents();
    void update();
    void draw();

    bool tryMove(int dx, int dy);
    void tryRotate();
    void hardDrop();
    void lockCurrentPiece();
    void spawnNextPiece();
    void restart();

    float dropIntervalSeconds() const;
    void addScoreForLines(int cleared);

    sf::Color colorFor(int value) const;
    void drawBlock(int gridX, int gridY, sf::Color color, float offsetX, float offsetY, float size = CellSize);
    void drawBoard();
    void drawCurrentPiece();
    void drawPreview();
    void drawSidePanel();

    int loadHighScore() const;
    void saveHighScore() const;
    void updateWindowTitle();
};
