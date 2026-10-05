#include "Game.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>

Game::Game()
    : window_(sf::VideoMode(620, 680), "Tetris"),
      current_(Tetromino::T),
      next_(Tetromino::T),
      rng_(std::random_device{}()) {
    window_.setFramerateLimit(60);
    highScore_ = loadHighScore();
    current_ = randomPiece();
    next_ = randomPiece();
    updateWindowTitle();
}

void Game::run() {
    while (window_.isOpen()) {
        handleEvents();
        update();
        draw();
    }
}

Piece Game::randomPiece() {
    std::uniform_int_distribution<int> dist(1, 7);
    return Piece(static_cast<Tetromino>(dist(rng_)), 0, 3, -1);
}

void Game::handleEvents() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
        if (event.type != sf::Event::KeyPressed) {
            continue;
        }

        if (event.key.code == sf::Keyboard::Escape) {
            window_.close();
        }

        if (gameOver_) {
            if (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::Enter) {
                restart();
            }
            continue;
        }

        switch (event.key.code) {
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
                tryMove(-1, 0);
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                tryMove(1, 0);
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                if (tryMove(0, 1)) {
                    ++score_;
                    updateWindowTitle();
                }
                break;
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                tryRotate();
                break;
            case sf::Keyboard::Space:
                hardDrop();
                break;
            default:
                break;
        }
    }
}

void Game::update() {
    if (gameOver_) {
        return;
    }

    if (fallClock_.getElapsedTime().asSeconds() >= dropIntervalSeconds()) {
        if (!tryMove(0, 1)) {
            lockCurrentPiece();
        }
        fallClock_.restart();
    }
}

void Game::draw() {
    window_.clear(sf::Color(22, 24, 31));
    drawBoard();
    drawCurrentPiece();
    drawPreview();
    drawSidePanel();
    window_.display();
}

bool Game::tryMove(int dx, int dy) {
    Piece moved = current_;
    moved.move(dx, dy);
    if (!board_.canPlace(moved)) {
        return false;
    }
    current_ = moved;
    return true;
}

void Game::tryRotate() {
    Piece rotated = current_.rotatedClockwise();
    const std::array<int, 5> kicks{0, -1, 1, -2, 2};

    for (int kick : kicks) {
        Piece candidate = rotated;
        candidate.move(kick, 0);
        if (board_.canPlace(candidate)) {
            current_ = candidate;
            return;
        }
    }
}

void Game::hardDrop() {
    int distance = 0;
    while (tryMove(0, 1)) {
        ++distance;
    }
    score_ += distance * 2;
    lockCurrentPiece();
    fallClock_.restart();
}

void Game::lockCurrentPiece() {
    const bool visible = board_.lockPiece(current_);
    const int cleared = board_.clearFullRows();
    addScoreForLines(cleared);

    if (!visible) {
        gameOver_ = true;
    } else {
        spawnNextPiece();
        if (!board_.canPlace(current_)) {
            gameOver_ = true;
        }
    }

    if (score_ > highScore_) {
        highScore_ = score_;
        saveHighScore();
    }
    updateWindowTitle();
}

void Game::spawnNextPiece() {
    current_ = next_;
    current_.setPosition(3, -1);
    next_ = randomPiece();
}

void Game::restart() {
    board_.reset();
    score_ = 0;
    lines_ = 0;
    level_ = 1;
    gameOver_ = false;
    current_ = randomPiece();
    next_ = randomPiece();
    fallClock_.restart();
    updateWindowTitle();
}

float Game::dropIntervalSeconds() const {
    return std::max(0.08f, 0.75f - static_cast<float>(level_ - 1) * 0.06f);
}

void Game::addScoreForLines(int cleared) {
    if (cleared <= 0) {
        return;
    }

    static const std::array<int, 5> base{0, 100, 300, 500, 800};
    score_ += base[std::min(cleared, 4)] * level_;
    lines_ += cleared;
    level_ = 1 + lines_ / 10;
}

sf::Color Game::colorFor(int value) const {
    switch (value) {
        case 1: return sf::Color(70, 220, 235);   // I
        case 2: return sf::Color(240, 220, 70);   // O
        case 3: return sf::Color(170, 90, 220);   // T
        case 4: return sf::Color(80, 200, 110);   // S
        case 5: return sf::Color(225, 75, 75);    // Z
        case 6: return sf::Color(80, 115, 230);   // J
        case 7: return sf::Color(240, 150, 65);   // L
        default: return sf::Color(45, 48, 58);
    }
}

void Game::drawBlock(int gridX, int gridY, sf::Color color, float offsetX, float offsetY, float size) {
    sf::RectangleShape block(sf::Vector2f(size - 2.f, size - 2.f));
    block.setPosition(offsetX + gridX * size + 1.f, offsetY + gridY * size + 1.f);
    block.setFillColor(color);
    block.setOutlineThickness(1.f);
    block.setOutlineColor(sf::Color(255, 255, 255, 35));
    window_.draw(block);
}

void Game::drawBoard() {
    sf::RectangleShape border(sf::Vector2f(Board::Width * CellSize + 4.f, Board::Height * CellSize + 4.f));
    border.setPosition(BoardOffsetX - 2.f, BoardOffsetY - 2.f);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(sf::Color(120, 125, 145));
    window_.draw(border);

    for (int y = 0; y < Board::Height; ++y) {
        for (int x = 0; x < Board::Width; ++x) {
            const int value = board_.at(x, y);
            drawBlock(x, y, colorFor(value), BoardOffsetX, BoardOffsetY);
        }
    }
}

void Game::drawCurrentPiece() {
    for (const Cell& cell : current_.cells()) {
        if (cell.y >= 0) {
            drawBlock(cell.x, cell.y, colorFor(static_cast<int>(current_.type())), BoardOffsetX, BoardOffsetY);
        }
    }
}

void Game::drawPreview() {
    const float previewX = 400.f;
    const float previewY = 115.f;

    sf::RectangleShape panel(sf::Vector2f(165.f, 150.f));
    panel.setPosition(previewX - 20.f, previewY - 25.f);
    panel.setFillColor(sf::Color(31, 34, 43));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(90, 95, 110));
    window_.draw(panel);

    Piece preview = next_;
    preview.setPosition(0, 0);
    for (const Cell& cell : preview.cells()) {
        drawBlock(cell.x, cell.y, colorFor(static_cast<int>(preview.type())), previewX, previewY, 26.f);
    }
}

void Game::drawSidePanel() {
    // No external font file is needed. The numeric state is kept in the window title.
    // These bars give a quick visual indication of level and game-over state.
    sf::RectangleShape levelBar(sf::Vector2f(std::min(level_, 10) * 14.f, 16.f));
    levelBar.setPosition(380.f, 330.f);
    levelBar.setFillColor(sf::Color(110, 200, 255));
    window_.draw(levelBar);

    sf::RectangleShape levelOutline(sf::Vector2f(140.f, 16.f));
    levelOutline.setPosition(380.f, 330.f);
    levelOutline.setFillColor(sf::Color::Transparent);
    levelOutline.setOutlineThickness(1.f);
    levelOutline.setOutlineColor(sf::Color(120, 125, 145));
    window_.draw(levelOutline);

    if (gameOver_) {
        sf::RectangleShape overlay(sf::Vector2f(Board::Width * CellSize, Board::Height * CellSize));
        overlay.setPosition(BoardOffsetX, BoardOffsetY);
        overlay.setFillColor(sf::Color(180, 40, 40, 85));
        window_.draw(overlay);
    }
}

int Game::loadHighScore() const {
    std::ifstream in("highscore.txt");
    int value = 0;
    if (in >> value && value >= 0) {
        return value;
    }
    return 0;
}

void Game::saveHighScore() const {
    std::ofstream out("highscore.txt", std::ios::trunc);
    if (out) {
        out << highScore_ << '\n';
    }
}

void Game::updateWindowTitle() {
    std::ostringstream title;
    title << "Tetris | Score: " << score_
          << " | Lines: " << lines_
          << " | Level: " << level_
          << " | High: " << highScore_;
    if (gameOver_) {
        title << " | GAME OVER - Press R to restart";
    } else {
        title << " | Arrows/WASD, Space = hard drop";
    }
    window_.setTitle(title.str());
}
