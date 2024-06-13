/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** pacman
*/

#include "Pacman.hpp"

namespace arc {

PacmanGame::PacmanGame() : _score(0), _gameOver(false), _elapsedTimeSinceLastMove(0), _elapsedTimeSinceLastPacmanMove(0), _ghostMoveDelay(5.0f), _pacmanMoveDelay(3.0f), _ghostsInitialDelay(100.0f) {
    srand(time(NULL));
    reset();
}

PacmanGame::~PacmanGame() {}

void PacmanGame::reset() {
    _map = {
        "#####################",
        "#.........#.........#",
        "#.###.###.#.###.###.#",
        "#.###.###.#.###.###.#",
        "#...................#",
        "#.###.#.#####.#.###.#",
        "#.....#...#...#.....#",
        "#####.### # ###.#####",
        "    #.#       #.#    ",
        "    #.#       #.#    ",
        "    #.#       #.#    ",
        "    #.#       #.#    ",
        "#####.# ## ## #.#####",
        "#........#..........#",
        "#.###.###..####.###.#",
        "#...#...........#...#",
        "###.#.#.#####.#.#.###",
        "#.....#...#...#.....#",
        "#.#######.#.#######.#",
        "#...................#",
        "#####################"
    };

    _pacmanPosition = {13, 10};
    _pacmanDirection = RIGHT;
    _score = 0;
    _gameOver = false;
    _lives = 3;
    _ghostPositions = {{9, 8}, {9, 9}, {10, 8}, {10, 9}};
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
            _score += 1;
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
    _ghostsInitialDelay -= elapsed;
    if (_ghostsInitialDelay > 0) {
        return;
    }

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

    if (_score == 169) {
        reset();
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

    _elapsedTimeSinceLastPacmanMove += elapsed;
    if (_elapsedTimeSinceLastPacmanMove >= _pacmanMoveDelay) {
        movePacman();
        _elapsedTimeSinceLastPacmanMove = 0;
    }

    updateGhosts(elapsed);
    checkCollisions();
}

void drawText(arc::IScreen& screen, int y, std::string str) {
    for (size_t i = 0; i < str.size(); ++i) {
        arc::IScreen::Tile tile;
        tile.textCharacters = {str[i], ' '};
        tile.textColor = arc::Color::ColorBlack;
        screen.setTile(i, y, tile);
    }
}

void PacmanGame::draw(arc::IScreen& screen) {
    screen.setSize(50, 50);
    auto [width, height] = screen.getSize();

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            arc::IScreen::Tile emptyTile;
            emptyTile.textCharacters = {' ', ' '};
            //emptyTile.texturePath = "Assets/Images/background_empty.png";
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
                    tile.texturePath = "Assets/Images/pacman_border.png";
                    tile.textColor = arc::Color::ColorBlue;
                    break;
                case '.':
                    tile.textCharacters = {'.', ' '};
                    tile.texturePath = "Assets/Images/dot_pacman.png";
                    tile.textColor = arc::Color::ColorWhite;
                    break;
                default:
                    tile.textCharacters = {' ', ' '};
                    tile.texturePath = "Assets/Images/background_empty.png";
                    tile.textColor = arc::Color::ColorBlack;
                    break;
            }
            screen.setTile(x, y, tile);
        }
    }
    drawText(screen, 23, "Score: " + std::to_string(_score));
    drawText(screen, 25, "Lifes: " + std::to_string(_lives));
    drawText(screen, 27, "Keybinds: < ^ v >");
    // Draw Pacman
    arc::IScreen::Tile pacmanTile;
    pacmanTile.textCharacters = {'P', ' '};
    pacmanTile.textColor = arc::Color::ColorYellow;
    pacmanTile.texturePath = "Assets/Images/pacman.png";
    screen.setTile(_pacmanPosition.second, _pacmanPosition.first, pacmanTile);
    
    // Draw Ghosts
    arc::IScreen::Tile ghostTile;
    ghostTile.textCharacters = {'G', ' '};
    ghostTile.textColor = arc::Color::ColorRed;
    ghostTile.texturePath = "Assets/Images/ghost.png";
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