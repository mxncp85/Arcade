/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** ncurses
*/

#include <ncurses.h>
#include "../../../common/IGraphical.hpp"

namespace arc
{
    class Ncurses : public IGraphical
    {
    public:
        Ncurses() {
            initscr();
            noecho();
            keypad(stdscr, TRUE);
            raw();
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
                case ' ':
                    eventList.push_back(arc::Event::EventAction);
                    break;
                case 'q':
                    eventList.push_back(arc::Event::EventExit);
                    break;
                default:
                    break;
            }
            return eventList;
        }

        void draw(const arc::IScreen& screen) override {
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

extern "C" arc::IGraphical* create() {
    return new arc::Ncurses();
}