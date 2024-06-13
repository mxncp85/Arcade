/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** ncurses
*/

#ifndef NCURSES_H_
    #define NCURSES_H_

    #include <ncurses.h>
    #include <iostream>
    #include <list>
    #include "../../../common/IGraphical.hpp"

namespace arc
{
    class Ncurses : public IGraphical
    {
    public:
        Ncurses(const arc::IScreen& screen);
        ~Ncurses();

        std::list<arc::Event> events() override;
        void draw(const arc::IScreen& screen) override;
    };
}

extern "C" arc::IGraphical* create(const arc::IScreen& screen);

#endif /* !NCURSES_H_ */
