/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** ncurses
*/

#include "Ncurses.hpp"

namespace arc
{

Ncurses::Ncurses(const arc::IScreen& screen)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
}

Ncurses::~Ncurses()
{
    endwin();
}

std::list<arc::Event> Ncurses::events()
{
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

void Ncurses::draw(const arc::IScreen& screen) 
{
    clear();
    for (unsigned int y = 0; y < screen.getSize().second; ++y) {
        for (unsigned int x = 0; x < screen.getSize().first; ++x) {
            const auto& tile = screen.getTile(x, y);
            mvaddch(y, x, tile.textCharacters.first);
        }
    }
    refresh();
}
}

extern "C" arc::IGraphical* create(const arc::IScreen& screen)
{
    return new arc::Ncurses(screen);
}
