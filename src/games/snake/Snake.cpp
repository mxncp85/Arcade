/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** snake
*/

#include "Snake.hpp"
#include <cstdlib>
#include <ctime>

arc::SnakeGame::SnakeGame() : _score(0), _direction(RIGHT), _gameOver(false) {
    reset();
}

arc::SnakeGame::~SnakeGame() {}

void arc::SnakeGame::reset() {
    _score = 0;
    _direction = RIGHT;
    _snake.clear();
    _snake.push_back({5, 5});
    _snake.push_back({5, 4});
    _snake.push_back({5, 3});
    _food = {10, 10};
    _gameOver = false;
}

void arc::SnakeGame::moveSnake() {
    auto head = _snake.front();
    switch (_direction) {
        case UP: head.first--; break;
        case DOWN: head.first++; break;
        case LEFT: head.second--; break;
        case RIGHT: head.second++; break;
    }
    _snake.push_front(head);
    if (head == _food) {
        _score++;
        _food = {rand() % 20, rand() % 20};
    } else {
        _snake.pop_back();
    }
    checkCollision();
}

void arc::SnakeGame::checkCollision() {
    auto head = _snake.front();
    if (head.first < 0 || head.first >= 20 || head.second < 0 || head.second >= 20) {
        _gameOver = true;
    }
    for (size_t i = 1; i < _snake.size(); ++i) {
        if (_snake[i] == head) {
            _gameOver = true;
            break;
        }
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

    moveSnake();
}


void arc::SnakeGame::draw(arc::IScreen& screen) {
    for (const auto& segment : _snake) {
        arc::IScreen::Tile tile;
        tile.textCharacters = {'O', ' '};
        screen.setTile(segment.second, segment.first, tile);
    }
    arc::IScreen::Tile foodTile;
    foodTile.textCharacters = {'X', ' '};
    screen.setTile(_food.second, _food.first, foodTile);
}

unsigned int arc::SnakeGame::score() const {
    return _score;
}

extern "C" arc::IGame* create() {
    return new arc::SnakeGame();
}