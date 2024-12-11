#include <assert.h>
#include <iostream>
#include <time.h>
#include <dlfcn.h>

#include "implementation/sfm.hpp"
#include "api/api_photoshop.hpp"
#include "api/api_actions.hpp"

#define DLL_CHECK(pointer)          if (pointer == nullptr) \
                                    {                       \
                                        std::cerr << "Error: " << dlerror() << std::endl; \
                                        return 1;           \
                                    }


/*static const std::vector<const char*> PLUGIN_NAMES = {"build/canvas.dll", "build/toolbar.dll",
                                                      "build/brush.dll", "build/geometry.dll",
                                                      "build/optionbar.dll", "build/filters.dll"};*/
static const std::vector<const char*> PLUGIN_NAMES = {"build/canvas.dll", "build/toolbar.dll",
                                                      "build/brush.dll", "build/geometry.dll",
                                                      "build/optionbar.dll"};
static       std::vector<void*> dll_ptrs;

static const char* LOAD_PLUGIN   = "onLoadPlugin";
static const char* UNLOAD_PLUGIN = "onUnloadPlugin";

int main()
{
    psapi::sfm::RenderWindow window(1200, 800, "PhotoRedactor");

    psapi::IRootWindow* root = psapi::getRootWindow();
    psapi::AActionController* controller = psapi::getActionController();

    for (auto& plugin_name : PLUGIN_NAMES)
    {
        void* so_lib = dlopen(plugin_name, RTLD_NOW);
        DLL_CHECK(so_lib);

        bool (*onLoadPlugin)() = (bool (*)()) dlsym(so_lib, LOAD_PLUGIN);
        DLL_CHECK(onLoadPlugin);

        dll_ptrs.push_back(so_lib);

        onLoadPlugin();
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

        controller->execute(root->createAction(&window, event));
        root->draw(&window);

        //sprite.draw(&window);

        window.display();
        window.clear();
    }

    for (int i = dll_ptrs.size() - 1; i >= 0; i--)
    {
        void* so_lib = dll_ptrs[i];

        bool (*onUnloadPlugin)() = (bool (*)()) dlsym(so_lib, UNLOAD_PLUGIN);
        DLL_CHECK(onUnloadPlugin);

        onUnloadPlugin();
    }


    return 0;
}
