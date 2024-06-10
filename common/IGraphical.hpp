#pragma once

#include <list>

#include "IScreen.hpp"
#include "Enum.hpp"

namespace arc
{
    class IGraphical
    {
    public:
        class GraphicalException : public std::exception
        {
        private:
            std::string _message;

        public:
            GraphicalException(const std::string& message);
            ~GraphicalException();

            const char* what() const noexcept override;
        };
        
    public:
        IGraphical() = default;
        virtual ~IGraphical() = default;

        virtual std::list<arc::Event>   events() = 0;
        virtual void                    draw(const arc::IScreen& screen) = 0;
    };
}
