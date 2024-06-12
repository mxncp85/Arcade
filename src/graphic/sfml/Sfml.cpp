/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** sfml
*/

#include "Sfml.hpp"

namespace arc {

SfmlGraphical::SfmlGraphical(const arc::IScreen& screen)
{
    _window.create(sf::VideoMode(screen.getSize().first * 20, screen.getSize().second * 20), "Arcade SFML");
    _window.setFramerateLimit(60);
    
    initColorMap();
}

SfmlGraphical::~SfmlGraphical() {
    _window.close();
}

void SfmlGraphical::initColorMap() {
    _colorMap[arc::Color::ColorBlack] = sf::Color::Black;
    _colorMap[arc::Color::ColorRed] = sf::Color::Red;
    _colorMap[arc::Color::ColorGreen] = sf::Color::Green;
    _colorMap[arc::Color::ColorYellow] = sf::Color::Yellow;
    _colorMap[arc::Color::ColorBlue] = sf::Color::Blue;
    _colorMap[arc::Color::ColorMagenta] = sf::Color::Magenta;
    _colorMap[arc::Color::ColorCyan] = sf::Color::Cyan;
    _colorMap[arc::Color::ColorWhite] = sf::Color::White;
}

std::list<arc::Event> SfmlGraphical::events() {
    std::list<arc::Event> eventList;
    sf::Event event;

    while (_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                eventList.push_back(arc::Event::EventExit);
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        eventList.push_back(arc::Event::EventUp);
                        break;
                    case sf::Keyboard::Down:
                        eventList.push_back(arc::Event::EventDown);
                        break;
                    case sf::Keyboard::Left:
                        eventList.push_back(arc::Event::EventLeft);
                        break;
                    case sf::Keyboard::Right:
                        eventList.push_back(arc::Event::EventRight);
                        break;
                    case sf::Keyboard::S:
                        eventList.push_back(arc::Event::EventAction);
                        break;
                    case sf::Keyboard::Q:
                        eventList.push_back(arc::Event::EventExit);
                        break;
                    case sf::Keyboard::R:
                        eventList.push_back(arc::Event::EventRestart);
                        break;
                    case sf::Keyboard::N:
                        eventList.push_back(arc::Event::EventNextGraphical);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    return eventList;
}

void SfmlGraphical::draw(const arc::IScreen& screen) {
    _window.clear();
    sf::Font font;

    try {
        if (!font.loadFromFile("Assets/Fonts/arial.ttf")) {
            throw arc::GraphicalException("Cannot load font");
        }
    } catch (const arc::GraphicalException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(84);
    }

    for (unsigned int y = 0; y < screen.getSize().second; ++y) {
        for (unsigned int x = 0; x < screen.getSize().first; ++x) {
            const auto& tile = screen.getTile(x, y);

            sf::RectangleShape rectangle(sf::Vector2f(20, 20));
            rectangle.setPosition(x * 20, y * 20);
            rectangle.setFillColor(sf::Color::White);

            _window.draw(rectangle);

            if (!tile.texturePath.empty()) {
                sf::Texture texture;

                try {
                    if (!texture.loadFromFile(tile.texturePath)) {
                        throw arc::GraphicalException("Cannot load texture");
                    }
                } catch (const arc::GraphicalException& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    exit(84);
                }
                
                sf::Sprite sprite(texture);
                sprite.setPosition(x * 20, y * 20);
                _window.draw(sprite);
            } else if (tile.textCharacters.first != ' ') {
                sf::Text text;
                text.setFont(font);
                text.setString(tile.textCharacters.first);
                text.setCharacterSize(20);
                text.setFillColor(_colorMap[tile.textColor]);
                text.setPosition(x * 20, y * 20);

                _window.draw(text);
            }
        }
    }

    _window.display();
}

} // namespace arc

extern "C" arc::IGraphical* create(const arc::IScreen& screen) {
    return new arc::SfmlGraphical(screen);
}
