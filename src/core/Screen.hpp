/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** screen
*/

#ifndef SCREEN_H_
    #define SCREEN_H_

    #include <vector>
    #include <stdexcept>
    #include "../../common/IScreen.hpp"

namespace arc {
    class Screen : public arc::IScreen {
        public:
            Screen();
            ~Screen();

            void setSize(unsigned int width, unsigned int height) override;
            std::pair<unsigned int, unsigned int> getSize() const override;
            void setTile(unsigned int x, unsigned int y, const Tile& tile) override;
            const Tile& getTile(unsigned int x, unsigned int y) const override;

        private:
            unsigned int _width;
            unsigned int _height;
            std::vector<std::vector<Tile>> _tiles;
    };
}

#include "Screen.hpp"

#endif /* !SCREEN_H_ */