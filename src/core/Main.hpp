/*
** EPITECH PROJECT, 2024
** bttf arcade
** File description:
** main
*/

#ifndef MAIN_H_
    #define MAIN_H_

#include <iostream>
#include <dlfcn.h>
#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <vector>
#include <string>
#include "../../common/IGame.hpp"
#include "../../common/IGraphical.hpp"
#include "Screen.hpp"
#include "Menu.hpp"

class IDLLoader {
public:
    virtual ~IDLLoader() {}
    virtual void* loadLibrary(const std::string& libraryPath) = 0;
    virtual void* getFunction(void* library, const std::string& functionName) = 0;
    virtual void closeLibrary(void* library) = 0;
};

class DLLoader : public IDLLoader {
    public:
        void* loadLibrary(const std::string& libraryPath) override;
        void* getFunction(void* library, const std::string& functionName) override;
        void closeLibrary(void* library) override;
};

#endif /* !MAIN_H_ */
