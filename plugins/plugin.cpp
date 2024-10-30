#include <iostream>
#include <dlfcn.h>

#include "plugin.hpp"

static void* dll_lib = nullptr;

extern "C"
{

bool loadPlugin()
{
    std::cout << "Loading plugin" << std::endl;

    void* dll_lib = dlopen("libapi_photoshop.dll", RTLD_NOW);
    if (dll_lib == nullptr)
    {
        std::cerr << "Error: " << dlerror() << std::endl;
        return false;
    }

    psapi::Canvas canvas({0, 0}, {500, 500}, {1, 1});

    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

    root->addWindow(&canvas);

    return true;
}

void unloadPlugin()
{
    std::cout << "Unloading plugin" << std::endl;

    if (dll_lib != nullptr)
    {
        dlclose(dll_lib);
        dll_lib = nullptr;
    }
}

}
