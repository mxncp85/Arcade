/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** ncurses
*/

#include <ncurses.h>
#include "../../../common/IGame.hpp"

namespace arc
{
    class NcursesGame : public IGame
    {
    public:
        NcursesGame() {
            initscr(); // Initialize the library
            raw();     // Line buffering disabled
            keypad(stdscr, TRUE); // We get F1, F2 etc..
            noecho(); // Don't echo() while we do getch
        }

        ~NcursesGame() {
            endwin(); // End curses mode
        }

        void update(float elapsed, const std::list<arc::Event>& events) override {
            // Implement your game update logic here
        }

        void draw(arc::IScreen& screen) override {
            // Implement your drawing logic here
        }

        unsigned int score() const override {
            // Return the current score
            return 0;
        }
    };
}

extern "C" arc::IGame* createGame() {
    return new arc::NcursesGame();
}