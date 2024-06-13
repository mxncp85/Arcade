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
        Menu(std::vector<std::string> gamesNames, std::vector<std::string> graphicsNames);
        ~Menu();

        void update(float elapsed, const std::list<arc::Event>& events) override;
        void updateMenu(float elapsed, const std::list<arc::Event>& events, std::string *gamePath);
        void draw(arc::IScreen& screen) override;
        unsigned int score() const override;
        int getSelectedIndex() const { return _selectedIndex; };

    private:
        void loadLibs();
        std::vector<std::string> _libs;
        int _selectedIndex;
        std::vector<std::string> _gamesNames;
        std::vector<std::string> _graphicsNames;
    };
}

#endif /* !MENU_H_ */