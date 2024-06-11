/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** sfml
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "../../../common/IGraphical.hpp"

namespace arc
{
    class SfmlGraphical : public IGraphical
    {
    public:
        SfmlGraphical();
        ~SfmlGraphical();

        std::list<arc::Event> events() override;
        void draw(const arc::IScreen& screen) override;

    private:
        sf::RenderWindow _window;
        std::map<arc::Color, sf::Color> _colorMap;
        void initColorMap();
    };
}
