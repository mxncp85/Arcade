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
#include <exception>
#include <string>
#include <cstdlib>
#include "../../../common/IGraphical.hpp"

namespace arc
{
    class SfmlGraphical : public IGraphical
    {
    public:
        SfmlGraphical(const arc::IScreen& screen);
        ~SfmlGraphical();

        std::list<arc::Event> events() override;
        void draw(const arc::IScreen& screen) override;

    private:
        sf::RenderWindow _window;
        std::map<arc::Color, sf::Color> _colorMap;
        void initColorMap();
    };

    class GraphicalException : public std::exception {
    private:
        std::string _message;

    public:
        explicit GraphicalException(const std::string& message) : _message(message) {}

        const char* what() const noexcept override {
            return _message.c_str();
        }
    };
}
