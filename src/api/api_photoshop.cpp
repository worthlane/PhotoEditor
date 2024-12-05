#include <iostream>

#include "api/api_photoshop.hpp"

static const size_t SCREEN_LENGTH = 1200;
static const size_t SCREEN_WIDTH  = 800;

namespace psapi
{

sfm::IntRect getCanvasIntRect()
{
    return {{0, 0}, {SCREEN_LENGTH, SCREEN_WIDTH}};
}

sfm::IntRect getToolbarIntRect()
{
    return {{0, 0}, {0, 0}};
}

sfm::IntRect getOptionsBarIntRect()
{
    return {{0, 0}, {0, 0}};
}

sfm::IntRect getInstrumentOptionsIntRect()
{
    return {{0, 0}, {0, 0}};
}

psapi::sfm::vec2u getScreenSize()
{
    return {SCREEN_LENGTH, SCREEN_WIDTH};
}

bool IWindowContainer::isWindowContainer() const
{
    return true;
};

generalFunction getGeneralFunction(const std::string& name)
{
    std::cout << "getGeneralFunction: " << name << std::endl;
    return nullptr;
}

}
