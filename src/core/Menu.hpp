/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** menu
*/

#ifndef MENU_H_
    #define MENU_H_

#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include "../../common/IGame.hpp"
#include "../../common/Enum.hpp"
#include "../../common/IScreen.hpp"

namespace arc
{
    class Menu : public IGame
    {
    public:
        Menu();
        ~Menu();

        void update(float elapsed, const std::list<arc::Event>& events) override;
        void updateMenu(float elapsed, const std::list<arc::Event>& events, std::string *gamePath);
        void draw(arc::IScreen& screen) override;
        unsigned int score() const override;

    private:
        void loadLibs();
        std::vector<std::string> _libs;
        int _selectedIndex;
    };
}


#endif /* !MENU_H_ */