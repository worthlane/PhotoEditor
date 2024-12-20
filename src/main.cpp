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


static const std::vector<const char*> PLUGIN_NAMES = {"build/canvas.dylib", "build/toolbar.dylib",
                                                      "build/optionbar.dylib", "build/menubar.dylib",
                                                      "build/brush.dylib", "build/geometry.dylib", "build/filters.dylib",
                                                      "build/files.dylib", "build/edit.dylib", "build/lib_unsharp_mask.dylib"};//, "build/layer.dylib", "build/help.dylib"};
static       std::vector<void*> dll_ptrs;

static const char* LOAD_PLUGIN   = "onLoadPlugin";
static const char* UNLOAD_PLUGIN = "onUnloadPlugin";

static const char* BACKGROUND_TEXTURE = "assets/textures/white.jpg";

int main()
{
    auto screen = psapi::getScreenSize();

    psapi::sfm::RenderWindow window(screen.x, screen.y, "PhotoRedactor");

    psapi::IRootWindow* root = psapi::getRootWindow();
    psapi::AActionController* controller = psapi::getActionController();

    psapi::sfm::ITexture* background = psapi::sfm::ITexture::create().release();
    background->loadFromFile(BACKGROUND_TEXTURE);

    psapi::sfm::ISprite* background_sprite = psapi::sfm::ISprite::create().release();
    background_sprite->setTextureRect({{0, 0}, {screen.x, screen.y}});
    background_sprite->setTexture(background);
    background_sprite->setColor(psapi::sfm::Color(23, 23, 23));

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

        background_sprite->draw(&window);
        root->draw(&window);

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
