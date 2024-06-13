/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** menu
*/

#include "Menu.hpp"

namespace fs = std::filesystem;

namespace arc {

Menu::Menu(std::vector<std::string> gamesNames, std::vector<std::string> graphicsNames) : _selectedIndex(0) {
    _gamesNames = gamesNames;
    _graphicsNames = graphicsNames;
}

Menu::~Menu() {}

void Menu::loadLibs() {
    _libs.clear();
    for (const auto& entry : fs::directory_iterator("./lib/")) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
            _libs.push_back(entry.path().filename().string());
        }
    }
}

void Menu::update(float elapsed, const std::list<arc::Event>& events) {}

void Menu::updateMenu(float elapsed, const std::list<arc::Event>& events, std::string *gamePath) {
    for (const auto& event : events) {
        switch (event) {
            case arc::Event::EventUp:
                if (_selectedIndex > 0) {
                    _selectedIndex--;
                }
                break;
            case arc::Event::EventDown:
                if (_selectedIndex < _gamesNames.size() - 1) {
                    _selectedIndex++;
                }
                break;
            case arc::Event::EventAction:
                // Handle selection action here
                if (gamePath && _selectedIndex < _gamesNames.size()) {
                    *gamePath = _gamesNames[_selectedIndex];
                }
                break;
            case arc::Event::EventExit:
                // Handle exit action here
                break;
            default:
                break;
        }
    }
}

void Menu::draw(arc::IScreen& screen) {
    screen.setSize(50, 50);
    for (unsigned int y = 0; y < screen.getSize().second; ++y) {
        for (unsigned int x = 0; x < screen.getSize().first; ++x) {
            arc::IScreen::Tile emptyTile;
            emptyTile.textCharacters = {' ', ' '};
            screen.setTile(x, y, emptyTile);
        }
    }

    auto size = screen.getSize();
    std::string menuStr = "|+ - ARCADE - +|";
    int startX = 5;
    int startY = 0;
    for (size_t y = 0; y < menuStr.size(); ++y) {
        arc::IScreen::Tile tile;
        tile.textCharacters = {menuStr[y], ' '};
        tile.textColor = arc::Color::ColorRed;
        screen.setTile(startX + y, startY, tile);
    }

    for (size_t i = 0; i < _gamesNames.size(); ++i) {
        arc::IScreen::Tile tile;

        tile.textCharacters = { '*', ' ' };
        if (i == _selectedIndex) {
            tile.textCharacters.first = '>';
            tile.textColor = arc::Color::ColorGreen;
        }
        screen.setTile(1, i + 1, tile);
        
        auto size = screen.getSize();
        std::string gameStr = _gamesNames[i];
        int startX = 5;
        int startY = i + 1;

        for (size_t y = 0; y < gameStr.size(); ++y) {
            arc::IScreen::Tile tile;
            tile.textCharacters = {gameStr[y], ' '};
            tile.textColor = arc::Color::ColorRed;
            screen.setTile(startX + y, startY, tile);
        }
    }
}

unsigned int Menu::score() const {
    return 0;
}

}

extern "C" arc::IGame* create(std::vector<std::string> gamesNames, std::vector<std::string> graphicsNames) {
    return new arc::Menu(gamesNames, graphicsNames);
}