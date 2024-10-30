#include "api/awindows.hpp"

namespace psapi
{

// ********************** AWINDOW *************************

IWindow* AWindow::getWindowById(wid_t id)
{
    return (id == id_) ? this : nullptr;
}

const IWindow* AWindow::getWindowById(wid_t id) const
{
    return (id == id_) ? static_cast<const IWindow*>(this) : nullptr;
}

// ********************** AWINDOW CONTAINER *************************



// ********************** AWINDOW VECTOR *************************

void AWindowVector::addWindow(std::unique_ptr<IWindow> window)
{
    windows_.push_back(std::move(window));
}

void AWindowVector::removeWindow(wid_t id)
{
    for (size_t i = 0; i < windows_.size(); i++)
    {
        if (windows_[i]->getId() == id)
        {
            windows_.erase(windows_.begin() + i);
            break;
        }
    }
}

IWindow* AWindowVector::getWindowById(wid_t id)
{
    for (auto& window : windows_)
    {
        if (window->getId() == id)
            return window.get();
    }

    return nullptr;
}

const IWindow* AWindowVector::getWindowById(wid_t id) const
{
    for (auto& window : windows_)
    {
        if (window->getId() == id)
            return window.get();
    }

    return nullptr;
}

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
{}

void RootWindow::forceDeactivate()
{}

void RootWindow::forceActivate()
{}

} // namespace psapi
