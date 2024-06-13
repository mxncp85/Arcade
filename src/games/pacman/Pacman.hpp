/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** pacman
*/

#ifndef PACMAN_H_
    #define PACMAN_H_

    #include <vector>
    #include <algorithm>
    #include "../../../common/IGame.hpp"

namespace arc {

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class PacmanGame : public IGame {
public:
    PacmanGame();
    ~PacmanGame();

    void update(float elapsed, const std::list<arc::Event>& events) override;
    void draw(arc::IScreen& screen) override;
    unsigned int score() const override;

private:
    void reset();
    void movePacman();
    void checkCollisions();
    void updateGhosts(float elapsed);

    std::vector<std::string> _map;
    std::pair<int, int> _pacmanPosition;
    Direction _pacmanDirection;
    unsigned int _score;
    bool _gameOver;
    float _elapsedTimeSinceLastMove;
    float _ghostMoveDelay;
    std::vector<std::pair<int, int>> _ghostPositions;
    std::vector<Direction> _ghostDirections;
};

}

#endif /* !PACMAN_H_ */