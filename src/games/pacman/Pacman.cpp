/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** pacman
*/

#include "Pacman.hpp"

int _lives = 3;

namespace arc {

PacmanGame::PacmanGame() : _score(0), _gameOver(false), _elapsedTimeSinceLastMove(0), _ghostMoveDelay(1.0f) {
    reset();
}

PacmanGame::~PacmanGame() {}

void PacmanGame::reset() {
    _map = {
        "#####################",
        "#........#..........#",
        "#.###.###.#.###.###.#",
        "#.###.###.#.###.###.#",
        "#...................#",
        "#.###.#.#####.#.###.#",
        "#.....#...#...#.....#",
        "#####.### # ###.#####",
        "    #.#       #.#    ",
        "#####.# ## ## #.#####",
        "#........#.........#",
        "#.###.###.#.###.###.#",
        "#...#...........#...#",
        "###.#.#.#####.#.#.###",
        "#.....#...#...#.....#",
        "#.#######.#.#######.#",
        "#...................#",
        "####################"
    };

    _pacmanPosition = {9, 9};
    _pacmanDirection = RIGHT;
    _score = 0;
    _gameOver = false;

    _ghostPositions = {{8, 8}, {8, 10}, {10, 8}, {10, 10}};
    _ghostDirections = {LEFT, RIGHT, UP, DOWN};
}

void PacmanGame::movePacman() {
    std::pair<int, int> nextPosition = _pacmanPosition;

    switch (_pacmanDirection) {
        case UP: nextPosition.first--; break;
        case DOWN: nextPosition.first++; break;
        case LEFT: nextPosition.second--; break;
        case RIGHT: nextPosition.second++; break;
    }

    if (_map[nextPosition.first][nextPosition.second] != '#') {
        _pacmanPosition = nextPosition;
        if (_map[_pacmanPosition.first][_pacmanPosition.second] == '.') {
            _map[_pacmanPosition.first][_pacmanPosition.second] = ' ';
            _score += 10;
        }
    }
}

void PacmanGame::checkCollisions() {
    for (const auto& ghost : _ghostPositions) {
        if (_pacmanPosition == ghost) {
            _lives--;
            if (_lives == 0) {
                _gameOver = true;
            }
            return;
        }
    }
}

void PacmanGame::updateGhosts(float elapsed) {
    _elapsedTimeSinceLastMove += elapsed;

    if (_elapsedTimeSinceLastMove < _ghostMoveDelay) {
        return;
    }

    _elapsedTimeSinceLastMove = 0;

    for (size_t i = 0; i < _ghostPositions.size(); ++i) {
        std::pair<int, int> nextPosition = _ghostPositions[i];

        switch (_ghostDirections[i]) {
            case UP: nextPosition.first--; break;
            case DOWN: nextPosition.first++; break;
            case LEFT: nextPosition.second--; break;
            case RIGHT: nextPosition.second++; break;
        }

        if (_map[nextPosition.first][nextPosition.second] != '#') {
            _ghostPositions[i] = nextPosition;
        } else {
            // Change direction if ghost hits a wall
            _ghostDirections[i] = static_cast<Direction>(rand() % 4);
        }
    }
}

void PacmanGame::update(float elapsed, const std::list<arc::Event>& events) {
    if (_gameOver) {
        return;
    }

    for (const auto& event : events) {
        switch (event) {
            case arc::Event::EventUp: _pacmanDirection = UP; break;
            case arc::Event::EventDown: _pacmanDirection = DOWN; break;
            case arc::Event::EventLeft: _pacmanDirection = LEFT; break;
            case arc::Event::EventRight: _pacmanDirection = RIGHT; break;
            case arc::Event::EventRestart: reset(); break;
            case arc::Event::EventExit: _gameOver = true; break;
            default: break;
        }
    }

    movePacman();
    updateGhosts(elapsed);
    checkCollisions();
}

void PacmanGame::draw(arc::IScreen& screen) {
    auto [width, height] = screen.getSize();

    for (unsigned int y = 0; y < 50; ++y) {
        for (unsigned int x = 0; x < 50; ++x) {
            arc::IScreen::Tile emptyTile;
            emptyTile.textCharacters = {' ', ' '};
            screen.setTile(x, y, emptyTile);
        }
    }
    
    // Ensure the screen size matches the map size
    if (_map.size() > height || _map[0].size() > width) {
        screen.setSize(_map[0].size(), _map.size());
    }

    for (unsigned int y = 0; y < _map.size(); ++y) {
        for (unsigned int x = 0; x < _map[y].size(); ++x) {
            arc::IScreen::Tile tile;
            switch (_map[y][x]) {
                case '#':
                    tile.textCharacters = {'#', ' '};
                    tile.textColor = arc::Color::ColorBlue;
                    break;
                case '.':
                    tile.textCharacters = {'.', ' '};
                    tile.textColor = arc::Color::ColorWhite;
                    break;
                default:
                    tile.textCharacters = {' ', ' '};
                    tile.textColor = arc::Color::ColorBlack;
                    break;
            }
            screen.setTile(x, y, tile);
        }
    }
    // Draw Pacman
    arc::IScreen::Tile pacmanTile;
    pacmanTile.textCharacters = {'P', ' '};
    pacmanTile.textColor = arc::Color::ColorYellow;
    screen.setTile(_pacmanPosition.second, _pacmanPosition.first, pacmanTile);
    
    // Draw Ghosts
    arc::IScreen::Tile ghostTile;
    ghostTile.textCharacters = {'G', ' '};
    ghostTile.textColor = arc::Color::ColorRed;
    for (const auto& ghost : _ghostPositions) {
        screen.setTile(ghost.second, ghost.first, ghostTile);
    }
}

unsigned int PacmanGame::score() const {
    return _score;
}

}

extern "C" arc::IGame* createGame() {
    return new arc::PacmanGame();
}