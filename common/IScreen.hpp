#pragma once

#include <utility>
#include <string>

#include "Enum.hpp"

namespace arc
{
    class IScreen
    {
    public:
        struct Tile
        {
            std::string texturePath;
            Orientation textureOrientation;

            std::pair<char, char>   textCharacters;
            Color                   textColor;
            Color                   textBackground;

            Tile() :
                texturePath(""), textureOrientation(arc::Orientation::Orientation0),
                textCharacters(' ', ' '), textColor(arc::Color::ColorWhite), textBackground(arc::Color::ColorBlack)
            {}
            
            ~Tile() = default;
        };
        
        IScreen() = default;
        virtual ~IScreen() = default;
        
        virtual void                                    setSize(unsigned int width, unsigned int height) = 0;
        virtual std::pair<unsigned int, unsigned int>   getSize() const = 0;
        virtual void                                    setTile(unsigned int x, unsigned int y, const Tile& tile) = 0;
        virtual const Tile&                             getTile(unsigned int x, unsigned int y) const = 0;
    };
}
