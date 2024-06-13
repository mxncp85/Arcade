/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** ncurses
*/

#include <ncurses.h>
#include <iostream>
#include <list>
#include "../../../common/IGraphical.hpp"

namespace arc
{
    class Ncurses : public IGraphical
    {
    public:
        Ncurses(const arc::IScreen& screen) {
            initscr();
            noecho();
            cbreak();
            keypad(stdscr, TRUE);
            curs_set(0);  // Hide cursor
            nodelay(stdscr, TRUE); // Make getch() non-blocking
        }

        ~Ncurses() {
            endwin();
        }

        std::list<arc::Event> events() override {
            std::list<arc::Event> eventList;
            int ch = getch();
            switch (ch) {
                case KEY_UP:
                    eventList.push_back(arc::Event::EventUp);
                    break;
                case KEY_DOWN:
                    eventList.push_back(arc::Event::EventDown);
                    break;
                case KEY_LEFT:
                    eventList.push_back(arc::Event::EventLeft);
                    break;
                case KEY_RIGHT:
                    eventList.push_back(arc::Event::EventRight);
                    break;
                case 's':
                    eventList.push_back(arc::Event::EventAction);
                    break;
                case 'q':
                    eventList.push_back(arc::Event::EventExit);
                    break;
                case 'r':
                    eventList.push_back(arc::Event::EventRestart);
                    break;
                case 'n':
                    eventList.push_back(arc::Event::EventNextGraphical);
                    break;
                case 'p':
                    eventList.push_back(arc::Event::EventNextGame);
                    break;
                case 'm':
                    eventList.push_back(arc::Event::EventBackToMenu);
                    break;
                default:
                    break;
            }
            return eventList;
        }

        void draw(const arc::IScreen& screen) override {
            clear();
            for (unsigned int y = 0; y < screen.getSize().second; ++y) {
                for (unsigned int x = 0; x < screen.getSize().first; ++x) {
                    const auto& tile = screen.getTile(x, y);
                    mvaddch(y, x, tile.textCharacters.first);
                }
            }
            refresh();
        }
    };
}

extern "C" arc::IGraphical* create(const arc::IScreen& screen) {
    return new arc::Ncurses(screen);
}