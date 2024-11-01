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


static const char* DYNAMIC_LIB_NAME = "plugin.dll";

static const char* BAR_TEMPLATE = "assets/textures/bar_template.png";

static const size_t LENGTH = 1200;
static const size_t WIDTH  = 800;

int main()
{
    void* so_lib = dlopen(DYNAMIC_LIB_NAME, RTLD_NOW);
    DLL_CHECK(so_lib);

    bool (*loadPlugin)() = (bool (*)()) dlsym(so_lib, "loadPlugin");
    DLL_CHECK(loadPlugin);

    bool (*unloadPlugin)() = (bool (*)()) dlsym(so_lib, "unloadPlugin");
    DLL_CHECK(unloadPlugin);

    psapi::sfm::RenderWindow window(LENGTH, WIDTH, "PhotoRedactor");

    psapi::sfm::Texture texture;
    texture.loadFromFile(BAR_TEMPLATE);

    psapi::sfm::Sprite sprite;

    sprite.setTexture(&texture);

    sprite.setPosition(0, 0);
    sprite.setScale(0.1, 0.1);

    //Canvas canvas({(LENGTH - 900) / 2, 150}, {900, 500}, {1, 1});

    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

    printf("--{%p}\n", root);

    loadPlugin();

    //root->addWindow(std::unique_ptr<psapi::IWindow>(&canvas));

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

        sprite.draw(&window);

        window.display();
        window.clear();
    }

    unloadPlugin();

    dlclose(so_lib);

    return 0;
}
