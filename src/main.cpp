#include <assert.h>
#include <iostream>
#include <time.h>
#include <dlfcn.h>

#include "graphics/my_sfml.hpp"
#include "api/canvas.hpp"

#define DLL_CHECK(pointer)          if (pointer == nullptr) \
                                    {                       \
                                        std::cerr << "Error: " << dlerror() << std::endl; \
                                        return 1;           \
                                    }


static const char* DYNAMIC_LIB_NAME = "libapi_photoshop.dll";

static const size_t LENGTH = 1280;
static const size_t WIDTH  = 720;

using namespace psapi;

int main()
{
    /*void* so_lib = dlopen(DYNAMIC_LIB_NAME, RTLD_NOW);
    DLL_CHECK(so_lib);

    bool (*loadPlugin)() = (bool (*)()) dlsym(so_lib, "loadPlugin");
    DLL_CHECK(loadPlugin);

    bool (*unloadPlugin)() = (bool (*)()) dlsym(so_lib, "unloadPlugin");
    DLL_CHECK(unloadPlugin);*/

    //loadPlugin();

    sfm::RenderWindow window(LENGTH, WIDTH, "PhotoRedactor");

    sfm::Image image;
    image.create(100, 100, psapi::sfm::RED);

    sfm::Texture texture;
    texture.update(&image);

    sfm::Sprite sprite;
    sprite.setTexture(&texture, true);
    sprite.setPosition(LENGTH / 2, WIDTH / 2);

    while (window.isOpen())
    {
        sfm::Event event;

        if (window.pollEvent(event))
        {
            if (event.type == sfm::Event::Closed)
            {
                window.close();
            }
        }

        sprite.draw(&window);

        window.display();
        window.clear();
    }

    //unloadPlugin();

    //dlclose(so_lib);

    return 0;
}
