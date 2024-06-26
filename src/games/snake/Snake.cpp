/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** snake
*/

#include "Snake.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace arc {

SnakeGame::SnakeGame()
    : _score(0), _direction(RIGHT), _gameOver(false), _speed(1.3f), _elapsedTimeSinceLastMove(0)
{
    srand(time(NULL));
    reset();
}

SnakeGame::~SnakeGame() {}

void SnakeGame::reset()
{
    _score = 0;
    _direction = RIGHT;
    _snake.clear();
    _snake.push_back({10, 10});
    _snake.push_back({10, 9});
    _snake.push_back({10, 8});
    _snake.push_back({10, 7});
    _gameOver = false;
    spawnFood();
}

void SnakeGame::moveSnake()
{
    auto head = _snake.front();
    switch (_direction) {
        case UP: head.first--; break;
        case DOWN: head.first++; break;
        case LEFT: head.second--; break;
        case RIGHT: head.second++; break;
    }

    if (head.first < 0 || head.first >= 21 || head.second < 0 || head.second >= 21) {
        _gameOver = true;
        return;
    }

    _snake.push_front(head);
    if (head == _food) {
        _score++;
        spawnFood();
    } else {
        _snake.pop_back();
    }
    checkCollision();
}

void SnakeGame::checkCollision()
{
    auto head = _snake.front();
    if (std::find(_snake.begin() + 1, _snake.end(), head) != _snake.end()) {
        _gameOver = true;
    }
}

void SnakeGame::update(float elapsed, const std::list<arc::Event>& events)
{
    for (const auto& event : events) {
        switch (event) {
            case arc::Event::EventLeft:
                switch (_direction) {
                    case UP: _direction = LEFT; break;
                    case DOWN: _direction = RIGHT; break;
                    case LEFT: _direction = DOWN; break;
                    case RIGHT: _direction = UP; break;
                }
                break;
            case arc::Event::EventRight:
                switch (_direction) {
                    case UP: _direction = RIGHT; break;
                    case DOWN: _direction = LEFT; break;
                    case LEFT: _direction = UP; break;
                    case RIGHT: _direction = DOWN; break;
                }
                break;
            case arc::Event::EventRestart:
                reset();
                break;
            case arc::Event::EventExit:
                _gameOver = true;
                break;
            default: break;
        }
    }

    if (_gameOver) {
        return;
    }

    _elapsedTimeSinceLastMove += elapsed;
    if (_elapsedTimeSinceLastMove >= _speed) {
        _elapsedTimeSinceLastMove = 0;
        moveSnake();
    }
}

void drawBorders(arc::IScreen& screen)
{
    auto size = screen.getSize();
    arc::IScreen::Tile borderTile;
    borderTile.textCharacters = {'/', ' '};
    borderTile.textColor = arc::Color::ColorMagenta;
    borderTile.texturePath = "Assets/Images/border.png";

    for (int x = 0; x < size.first; x++) {
        screen.setTile(x, 0, borderTile);
        screen.setTile(x, size.second - 1, borderTile);
    }

    for (int y = 0; y < size.second; y++) {
        screen.setTile(0, y, borderTile);
        screen.setTile(size.first - 1, y, borderTile);
    }
}

void drawScore(arc::IScreen& screen, unsigned int _score)
{
    auto size = screen.getSize();
    std::string scoreStr = "Score: " + std::to_string(_score);

    for (size_t i = 0; i < scoreStr.size(); ++i) {
        arc::IScreen::Tile tile;
        tile.textCharacters = {scoreStr[i], ' '};
        tile.textColor = arc::Color::ColorBlack;
        screen.setTile(i, size.second - 1, tile);
    }
}

void SnakeGame::draw(arc::IScreen& screen)
{
    screen.setSize(22,22);
    for (unsigned int y = 0; y < screen.getSize().second; ++y) {
        for (unsigned int x = 0; x < screen.getSize().first; ++x) {
            arc::IScreen::Tile emptyTile;
            emptyTile.textCharacters = {' ', ' '};
            screen.setTile(x, y, emptyTile);
        }
    }
    drawBorders(screen);
    drawScore(screen, _score);

    if (_gameOver) {
        auto size = screen.getSize();
        std::string gameOverStr = "Perdu";
        int startX = (size.first - gameOverStr.size()) / 2;
        int startY = size.second / 2;

        for (size_t i = 0; i < gameOverStr.size(); ++i) {
            arc::IScreen::Tile tile;
            tile.textCharacters = {gameOverStr[i], ' '};
            tile.textColor = arc::Color::ColorRed;
            screen.setTile(startX + i, startY, tile);
        }
    } else {
        for (const auto& segment : _snake) {
            arc::IScreen::Tile tile;
            tile.textCharacters = {'0', ' '};
            tile.textColor = arc::Color::ColorGreen;
            tile.texturePath = "Assets/Images/snake_body2.png";
            screen.setTile(segment.second, segment.first, tile);
        }

        arc::IScreen::Tile foodTile;
        foodTile.textCharacters = {'X', ' '};
        foodTile.textColor = arc::Color::ColorRed;
        foodTile.texturePath = "Assets/Images/apple.png";
        screen.setTile(_food.second, _food.first, foodTile);
    }
}

unsigned int SnakeGame::score() const
{
    return _score;
}

void SnakeGame::spawnFood()
{
    int x, y;
    do {
        x = rand() % 18 + 1;
        y = rand() % 18 + 1;
    } while (!isCellFree(x, y));
    _food = {x, y};
}

bool SnakeGame::isCellFree(int x, int y) const
{
    return std::find(_snake.begin(), _snake.end(), std::make_pair(y, x)) == _snake.end();
}

}

extern "C" arc::IGame* createGame()
{
    return new arc::SnakeGame();
}
