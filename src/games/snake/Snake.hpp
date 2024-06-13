/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** snake
*/

#ifndef SNAKE_H_
    #define SNAKE_H_

    #include <list>
    #include <deque>
    #include <utility>
    #include "../../../common/IGame.hpp"

namespace arc {
    class SnakeGame : public arc::IGame {
        public:
            enum Direction { UP, DOWN, LEFT, RIGHT };

            SnakeGame();
            ~SnakeGame();

            void update(float elapsed, const std::list<arc::Event>& events) override;
            void draw(arc::IScreen& screen) override;
            unsigned int score() const override;

        private:
            void reset();
            void moveSnake();
            void checkCollision();
            void spawnFood();
            bool isCellFree(int x, int y) const;

            unsigned int _score;
            Direction _direction;
            std::deque<std::pair<int, int>> _snake;
            std::pair<int, int> _food;
            bool _gameOver;
            float _speed;
            float _elapsedTimeSinceLastMove;
    };
}

#endif /* !SNAKE_H_ */
