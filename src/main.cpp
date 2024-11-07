#include <assert.h>
#include <iostream>
#include <time.h>
#include <dlfcn.h>

#include "graphics/my_sfml.hpp"
#include "api/utils.hpp"
#include "api/window_vector.hpp"
#include "api/root_window.hpp"

#define DLL_CHECK(pointer)          if (pointer == nullptr) \
                                    {                       \
                                        std::cerr << "Error: " << dlerror() << std::endl; \
                                        return 1;           \
                                    }


static const std::vector<const char*> PLUGIN_NAMES = {"build/canvas.dll", "build/toolbar.dll", "build/brush.dll"};
static       std::vector<void*> dll_ptrs;

static const char* LOAD_PLUGIN   = "loadPlugin";
static const char* UNLOAD_PLUGIN = "unloadPlugin";

static const size_t LENGTH = 1200;
static const size_t WIDTH  = 800;

int main()
{
    psapi::sfm::RenderWindow window(LENGTH, WIDTH, "PhotoRedactor");

    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

    for (auto& plugin_name : PLUGIN_NAMES)
    {
        void* so_lib = dlopen(plugin_name, RTLD_NOW);
        DLL_CHECK(so_lib);

        bool (*loadPlugin)() = (bool (*)()) dlsym(so_lib, LOAD_PLUGIN);
        DLL_CHECK(loadPlugin);

        dll_ptrs.push_back(so_lib);

        loadPlugin();
    }

    while (window.isOpen())
    {
        psapi::sfm::Event event;

        if (window.pollEvent(event))
        {
            if (event.type == psapi::sfm::Event::Closed)
            {
                window.close();
            }
        }

        root->update(&window, event);
        root->draw(&window);

        window.display();
        window.clear();
    }

    for (int i = dll_ptrs.size() - 1; i >= 0; i--)
    {
        void* so_lib = dll_ptrs[i];

        bool (*unloadPlugin)() = (bool (*)()) dlsym(so_lib, UNLOAD_PLUGIN);
        DLL_CHECK(unloadPlugin);

        unloadPlugin();
    }


    return 0;
}
