#pragma once

#include <list>

#include "Enum.hpp"
#include "IScreen.hpp"

namespace arc
{
    class IGame
    {
    public:
        class GameException : public std::exception
        {
        private:
            std::string _message;

        public:
            GameException(const std::string& message);
            ~GameException();

            const char* what() const noexcept override;
        };
        
    public:
        IGame() = default;
        virtual ~IGame() = default;

        virtual void            update(float elapsed, const std::list<arc::Event>& events) = 0;
        virtual void            draw(arc::IScreen& screen) = 0;
        virtual unsigned int    score() const = 0;
    };
}
