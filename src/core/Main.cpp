/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** main
*/

#include "Main.hpp"

std::vector<std::string> gamesNames;
std::vector<std::string> graphicsNames;
size_t currentGraphicalIndex = 0;
size_t currentGameIndex = 0;

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

arc::IGraphical* loadGraphicalLibrary(const std::string& libraryPath, const arc::IScreen& screen, DLLoader& loader) {
    void* graphicalLib;
    typedef arc::IGraphical* (*create_graphical_t)(const arc::IScreen&);
    create_graphical_t createFunc;
    arc::IGraphical* graphical;

    try {
        graphicalLib = loader.loadLibrary(libraryPath);
        if (!graphicalLib)
            throw std::runtime_error("Failed to load the library");

        createFunc = (create_graphical_t)loader.getFunction(graphicalLib, "create");
        if (!createFunc) {
            loader.closeLibrary(graphicalLib);
            throw std::runtime_error("Failed to load the factory function");
        }
        graphical = createFunc(screen);
    } catch (const std::exception& e) {
        std::cerr << "ARCADE - Error: " << e.what() << std::endl;
        loader.closeLibrary(graphicalLib);
        exit(84);
    }
    return graphical;
}

void runMenu(const std::vector<std::string>& gamesNames, const std::vector<std::string>& graphicsNames, arc::IGraphical* graphical, arc::Screen& screen, std::string& path) {
    arc::Menu menu(gamesNames, graphicsNames);

    while (1) {
        auto events = graphical->events();
        menu.updateMenu(0.16f, events, &path);
        menu.draw(screen);
        graphical->draw(screen);

        if (std::find(events.begin(), events.end(), arc::Event::EventExit) != events.end()) {
            exit(0);
            break;
        } else if (std::find(events.begin(), events.end(), arc::Event::EventAction) != events.end()) {
            currentGameIndex = menu.getSelectedIndex();
            break;
        }

        usleep(16000); // Wait for 16ms (~60fps)
    }
}

arc::IGame* loadGameLibrary(DLLoader& loader, const std::string& path, void*& gameLib) {
    gameLib = loader.loadLibrary(path);
    if (!gameLib)
        return nullptr;
    typedef arc::IGame* (*create_game_t)();
    create_game_t createFuncGame = (create_game_t)loader.getFunction(gameLib, "createGame");
    if (!createFuncGame) {
        std::cerr << "Failed to load the factory function" << std::endl;
        loader.closeLibrary(gameLib);
        return nullptr;
    }
    arc::IGame* game = createFuncGame();
    return game;
}

void switchToNextGraphicalLibrary(arc::IGraphical*& graphical, DLLoader& loader, arc::Screen& screen) {
    currentGraphicalIndex = currentGraphicalIndex + 1;
    if (currentGraphicalIndex >= graphicsNames.size())
        currentGraphicalIndex = 0;
    delete graphical;
    //loader.closeLibrary(graphical);
    graphical = loadGraphicalLibrary(graphicsNames[currentGraphicalIndex], screen, loader);
}

void switchToNextGameLibrary(arc::IGame*& game, DLLoader& loader, const std::string& path, void*& gameLib) {
    currentGameIndex = currentGameIndex + 1;
    if (currentGameIndex >= gamesNames.size())
        currentGameIndex = 0;
    delete game;
    game = loadGameLibrary(loader, gamesNames[currentGameIndex], gameLib);
}

void findGraphicalIndex(const std::vector<std::string>& graphicsNames, const std::string& path) {
    for (int i = 0; i < graphicsNames.size(); ++i) {
        if (graphicsNames[i] == path) {
            currentGraphicalIndex = i;
        }
    }
}

void runGameLoop(arc::IGraphical*& graphical, arc::IGame*& game, DLLoader& loader, arc::Screen& screen, std::string& path, void*& gameLib) {
    while (1)
    {
        auto events = graphical->events();
        game->update(0.16f, events);
        game->draw(screen);
        graphical->draw(screen);

        if (std::find(events.begin(), events.end(), arc::Event::EventExit) != events.end()) {
            break;
        } else if (std::find(events.begin(), events.end(), arc::Event::EventNextGraphical) != events.end()) {
            switchToNextGraphicalLibrary(graphical, loader, screen);
        }  else if (std::find(events.begin(), events.end(), arc::Event::EventNextGame) != events.end()) {
            switchToNextGameLibrary(game, loader, path, gameLib);
        } else if (std::find(events.begin(), events.end(), arc::Event::EventBackToMenu) != events.end()) {
            runMenu(gamesNames, graphicsNames, graphical, screen, path);
            delete game;
            game = loadGameLibrary(loader, path, gameLib);
        }
        usleep(16000); // Wait for 16ms (~60fps)
    }
}

int main(int ac,  char **av)
{
    if (ac == 2 && std::string(av[1]) == "-h") {
        std::cout << "USAGE:\n\t./arcade path_to_graphical_library.so\n" << std::endl;
        std::cout << "DESCRIPTION:\n\tarcade is a program that allows you to play games using different graphical libraries.\n" << std::endl;
        std::cout << "KEYS:\n\t- ARROW UP: Move up\n\t- ARROW DOWN: Move down\n\t- ARROW LEFT: Move left\n\t- ARROW RIGHT: Move right\n\t- S: Action/Select\n\t- Q: Exit the program\n\t- N: Switch to the next graphical library\n\t- P: Switch to the next game\n\t- M: Return to the menu" << std::endl;
        return 0;
    }
    if (ac != 2) {
        std::cerr << "USAGE: ./arcade path_to_graphical_library.so" << std::endl;
        return 84;
    }

    DLLoader loader;
    arc::Screen screen;
    screen.setSize(50, 50);

    // Graphic lib
    loadLibraries();
    arc::IGraphical* graphical = loadGraphicalLibrary(av[1], screen, loader);
    findGraphicalIndex(graphicsNames, av[1]);

    // Menu
    std::string path = " ";
    runMenu(gamesNames, graphicsNames, graphical, screen, path);
    if (path == " ")
        return 0;

    // Game
    void* gameLib = nullptr;
    arc::IGame* game = loadGameLibrary(loader, path, gameLib);
    if (!game) {
        return 84;
    }

    // Game loop
    runGameLoop(graphical, game, loader, screen, path, gameLib);
    
    delete graphical;
    delete game;
    loader.closeLibrary(gameLib);
    return 0;
}
