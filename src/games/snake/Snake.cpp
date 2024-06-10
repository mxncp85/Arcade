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

arc::SnakeGame::SnakeGame() : _score(0), _direction(RIGHT), _gameOver(false), _speed(0.1f), _elapsedTimeSinceLastMove(0) {
    srand(time(NULL));
    reset();
}

arc::SnakeGame::~SnakeGame() {}

void arc::SnakeGame::reset() {
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

void arc::SnakeGame::moveSnake() {
    auto head = _snake.front();
    switch (_direction) {
        case UP:
            head.first--;
            break;
        case DOWN:
            head.first++;
            break;
        case LEFT:
            head.second--;
            break;
        case RIGHT:
            head.second++;
            break;
    }

    // Check for collision with edges
    if (head.first < 0 || head.first >= 20 || head.second < 0 || head.second >= 20) {
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

void arc::SnakeGame::checkCollision() {
    auto head = _snake.front();
    // Check for collision with body
    if (std::find(_snake.begin() + 1, _snake.end(), head) != _snake.end()) {
        _gameOver = true;
    }
}

void arc::SnakeGame::update(float elapsed, const std::list<arc::Event>& events) {
    if (_gameOver) {
        return;
    }

    for (const auto& event : events) {
        switch (event) {
            case arc::Event::EventUp: if (_direction != DOWN) _direction = UP; break;
            case arc::Event::EventDown: if (_direction != UP) _direction = DOWN; break;
            case arc::Event::EventLeft: if (_direction != RIGHT) _direction = LEFT; break;
            case arc::Event::EventRight: if (_direction != LEFT) _direction = RIGHT; break;
            case arc::Event::EventRestart: reset(); break;
            case arc::Event::EventExit: _gameOver = true; break;
            default: break;
        }
    }

    _elapsedTimeSinceLastMove += elapsed;
    if (_elapsedTimeSinceLastMove >= _speed) {
        _elapsedTimeSinceLastMove = 0;
        moveSnake();
    }
}

void arc::SnakeGame::draw(arc::IScreen& screen) {
    for (const auto& segment : _snake) {
        arc::IScreen::Tile tile;
        tile.textCharacters = {' ', ' '};  // Effacer la queue
        screen.setTile(segment.second, segment.first, tile);
    }

    // Dessiner la tête du serpent
    arc::IScreen::Tile headTile;
    headTile.textCharacters = {'0', ' '};
    auto head = _snake.front();
    screen.setTile(head.second, head.first, headTile);

    // Dessiner la nourriture
    arc::IScreen::Tile foodTile;
    foodTile.textCharacters = {'X', ' '};
    screen.setTile(_food.second, _food.first, foodTile);
}

unsigned int arc::SnakeGame::score() const {
    return _score;
}

void arc::SnakeGame::spawnFood() {
    int x, y;
    do {
        x = rand() % 20;
        y = rand() % 20;
    } while (!isCellFree(x, y));
    _food = {x, y};
}

bool arc::SnakeGame::isCellFree(int x, int y) const {
    return std::find(_snake.begin(), _snake.end(), std::make_pair(y, x)) == _snake.end();
}

extern "C" arc::IGame* create() {
    return new arc::SnakeGame();
}