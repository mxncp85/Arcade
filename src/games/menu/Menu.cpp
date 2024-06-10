#include <vector>
#include "../../../common/IGame.hpp"
#include "../../../common/Enum.hpp"
#include "../../../common/IScreen.hpp"

namespace arc
{
    class Menu : public IGame
    {
    private:
        int selectedOption;
        std::vector<std::string> options;

    public:
        Menu() : selectedOption(0), options({"Option 1", "Option 2"}) {
        }

        ~Menu() {}

        void update(float elapsed, const std::list<arc::Event>& events) override {
            // Implement your menu update logic here
            // For example, you could change the selected option based on the user's input
            for (const auto& event : events) {
                if (event == Event::EventUp && selectedOption > 0) {
                    --selectedOption;
                } else if (event == Event::EventDown && selectedOption < options.size() - 1) {
                    ++selectedOption;
                }
            }
        }

        void draw(arc::IScreen& screen) override {
            for (size_t i = 0; i < options.size(); ++i) {
                arc::IScreen::Tile tile;
                tile.textCharacters = {options[i][0], ' '};
                tile.textColor = (i == selectedOption) ? arc::Color::ColorRed : arc::Color::ColorWhite;
                screen.setTile(0, i, tile);
            }
        }

        unsigned int score() const override {
            // The score doesn't really make sense for a menu, so we just return 0
            return 0;
        }
    };
}

extern "C" arc::IGame* create() {
    return new arc::Menu();
}