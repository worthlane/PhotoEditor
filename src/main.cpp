#include <assert.h>
#include <iostream>
#include <time.h>

#include "graphics/my_sfml.hpp"

static const double DELTA_ANGLE = 1e-4;
static const size_t LENGTH = 1280;
static const size_t WIDTH  = 720;
static const double SCALE  = 0.05;

using namespace psapi;

int main()
{
    sfm::RenderWindow window(LENGTH, WIDTH, "PhotoRedactor");

    sfm::Texture texture;
    texture.loadFromFile("assets/textures/blue_hovered.png");

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

    return 0;
}
