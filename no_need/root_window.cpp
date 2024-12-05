#include <iostream>

#include "api/root_window.hpp"

namespace psapi
{

// ********************** ROOT WINDOW *************************

IWindowContainer* getRootWindow()
{
    static RootWindow root_window;

    return &root_window;
}

void RootWindow::draw(IRenderWindow* renderWindow)
{
    for (auto& window : windows_)
    {
        window->draw(renderWindow);
    }
}

bool RootWindow::update(const IRenderWindow* renderWindow, const Event& event)
{
    bool flag = false;

    for (auto& window : windows_)
    {
        flag |= window->update(renderWindow, event);
    }

    return flag;
}

vec2i RootWindow::getPos() const
{
    return {0, 0};
}

bool RootWindow::isActive() const
{
    return true;
}

vec2u RootWindow::getSize() const
{
    static const size_t MAX_SIZE = 5000;

    return {MAX_SIZE, MAX_SIZE};
}
void RootWindow::setParent(const IWindow* parent)
{
}

void RootWindow::forceDeactivate()
{}

void RootWindow::forceActivate()
{}


}
