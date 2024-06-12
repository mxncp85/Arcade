/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** main
*/

#include "Main.hpp"

std::vector<std::string> gamesNames;
std::vector<std::string> graphicsNames;

void loadLibraries() {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir("./lib/")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = "./lib/" + std::string(ent->d_name);
            void* handle = dlopen(filename.c_str(), RTLD_LAZY);
            if (handle) {
                arc::IGame* (*createGame)();
                arc::IGraphical* (*createGraphical)();
                createGame = (arc::IGame* (*)())dlsym(handle, "createGame");
                if (createGame) {
                    gamesNames.push_back(filename);
                    dlclose(handle);
                    continue;
                }
                createGraphical = (arc::IGraphical* (*)())dlsym(handle, "create");
                if (createGraphical) {
                    graphicsNames.push_back(filename);
                }
                dlclose(handle);
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
    }
}

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
    arc::NcursesScreen screen;
    screen.setSize(50, 50);

    loadLibraries();

////////////////////////////////////////// GRAPHICAL
    void* graphicalLib;
    typedef arc::IGraphical* (*create_graphical_t)(const arc::IScreen&);
    create_graphical_t createFunc;
    arc::IGraphical* graphical;

try {
    graphicalLib = loader.loadLibrary(av[1]);
    if (!graphicalLib)
        return 84;
    
    createFunc = (create_graphical_t)loader.getFunction(graphicalLib, "create");
    if (!createFunc) {
        std::cerr << "Failed to load the factory function" << std::endl;
        loader.closeLibrary(graphicalLib);
        return 84;
    }
    graphical = createFunc(screen);
} catch (const std::exception& e) {
    std::cerr << "ARCADE - Error: " << e.what() << std::endl;
    return 84;
}
//////////////////////////////////////////


////////////////////////////////////////// MENU
    arc::Menu menu(gamesNames, graphicsNames);
    std::string path = " ";

    while (1) {
        auto events = graphical->events();
        menu.updateMenu(0.16f, events, &path);
        menu.draw(screen);
        graphical->draw(screen);

        if (std::find(events.begin(), events.end(), arc::Event::EventExit) != events.end()) {
            break;
        } else if (std::find(events.begin(), events.end(), arc::Event::EventAction) != events.end()) {
            break;
        }

        usleep(16000); // Wait for 16ms (~60fps)
    }
//////////////////////////////////////////

if (path == " ")
    return 0;

////////////////////////////////////////// GAME
    void* gameLib = loader.loadLibrary(path);
    if (!gameLib)
        return 84;
    typedef arc::IGame* (*create_game_t)();
    create_game_t createFuncGame = (create_game_t)loader.getFunction(gameLib, "createGame");
    if (!createFuncGame) {
        std::cerr << "Failed to load the factory function" << std::endl;
        loader.closeLibrary(gameLib);
        return 84;
    }
    arc::IGame* game = createFuncGame();
//////////////////////////////////////////

    while (1)
    {
        auto events = graphical->events();
        game->update(0.16f, events);
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
