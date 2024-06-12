/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** main
*/

#include "Main.hpp"

void *DLLoader::loadLibrary(const std::string &path)
{
    void* lib = dlopen(path.c_str(), RTLD_LAZY);
        if (!lib) {
            std::cerr << "Cannot open library: " << dlerror() << '\n';
        }
        return lib;
}

void* DLLoader::getFunction(void* library, const std::string& functionName)
{
    dlerror();
    void* func = dlsym(library, functionName.c_str());
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol '" << functionName << "': " << dlsym_error << '\n';
        dlclose(library);
    }
    return func;
}

void DLLoader::closeLibrary(void* library)
{
    dlclose(library);
}

int main(int ac,  char **av)
{
    if (ac != 2) {
        std::cerr << "Usage: ./arcade path_to_graphical_library.so" << std::endl;
        return 84;
    }

    DLLoader loader;

    void* graphicalLib = loader.loadLibrary(av[1]);
    if (!graphicalLib)
        return 84;
    typedef arc::IGraphical* (*create_graphical_t)();
    create_graphical_t createFunc = (create_graphical_t)loader.getFunction(graphicalLib, "create");
    if (!createFunc) {
        std::cerr << "Failed to load the factory function" << std::endl;
        loader.closeLibrary(graphicalLib);
        return 84;
    }
    arc::IGraphical* graphical = createFunc();

    void* gameLib = loader.loadLibrary("./arcade_snake.so");
    if (!gameLib)
        return 84;
    typedef arc::IGame* (*create_game_t)();
    create_game_t createFuncGame = (create_game_t)loader.getFunction(gameLib, "create");
    if (!createFuncGame) {
        std::cerr << "Failed to load the factory function" << std::endl;
        loader.closeLibrary(gameLib);
        return 84;
    }
    arc::IGame* game = createFuncGame();

    arc::NcursesScreen screen;
    screen.setSize(20, 20);

    while (1)
    {
        auto events = graphical->events();
        game->update(0.16f, events);
        screen.setSize(20,20);
        game->draw(screen);
        graphical->draw(screen);

        if (std::find(events.begin(), events.end(), arc::Event::EventExit) != events.end()) {
            break;
        }

        usleep(16000); // Wait for 16ms (~60fps)
    }
    
    delete graphical;
    delete game;
    loader.closeLibrary(graphicalLib);
    loader.closeLibrary(gameLib);

    return 0;
}
