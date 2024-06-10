/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** main
*/

#include "Screen.hpp"

namespace arc {
    NcursesScreen::NcursesScreen() : _width(0), _height(0) {}

    NcursesScreen::~NcursesScreen() {}

    void NcursesScreen::setSize(unsigned int width, unsigned int height) {
        _width = width;
        _height = height;
        _tiles.resize(height, std::vector<Tile>(width));
    }

    std::pair<unsigned int, unsigned int> NcursesScreen::getSize() const {
        return {_width, _height};
    }

    void NcursesScreen::setTile(unsigned int x, unsigned int y, const Tile& tile) {
        if (x < _width && y < _height) {
            _tiles[y][x] = tile;
        }
    }

    const arc::IScreen::Tile& NcursesScreen::getTile(unsigned int x, unsigned int y) const {
        if (x < _width && y < _height) {
            return _tiles[y][x];
        } else {
            throw std::out_of_range("Tile position out of range");
        }
    }

}