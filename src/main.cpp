#include <assert.h>
#include <iostream>
#include <time.h>
#include <dlfcn.h>

#include "graphics/my_sfml.hpp"
#include "api/canvas.hpp"
#include "api/utils.hpp"
#include "api/window_vector.hpp"
#include "api/root_window.hpp"

#define DLL_CHECK(pointer)          if (pointer == nullptr) \
                                    {                       \
                                        std::cerr << "Error: " << dlerror() << std::endl; \
                                        return 1;           \
                                    }


static const char* DYNAMIC_LIB_NAME = ".dll";

static const size_t LENGTH = 1280;
static const size_t WIDTH  = 720;

int main()
{
    /*void* so_lib = dlopen(DYNAMIC_LIB_NAME, RTLD_NOW);
    DLL_CHECK(so_lib);

    bool (*loadPlugin)() = (bool (*)()) dlsym(so_lib, "loadPlugin");
    DLL_CHECK(loadPlugin);

    bool (*unloadPlugin)() = (bool (*)()) dlsym(so_lib, "unloadPlugin");
    DLL_CHECK(unloadPlugin);*/

    //loadPlugin();

    psapi::sfm::RenderWindow window(LENGTH, WIDTH, "PhotoRedactor");

    psapi::sfm::Texture texture;
    texture.loadFromFile("assets/textures/blue_hovered.png");

    psapi::sfm::Sprite sprite;
    sprite.setTexture(&texture);
    sprite.setPosition(LENGTH / 2, WIDTH / 2);

    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

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

    //unloadPlugin();

    //dlclose(so_lib);

    return 0;
}
