#include "standard/api_photoshop.hpp"

namespace psapi
{

IWindow::~IWindow()
{}

bool IWindowContainer::isWindowContainer() const
{
    return true;
}

bool IWindowVector::isWindowContainer() const
{
    return true;
}

void IWindowVector::addWindow(std::unique_ptr<IWindow> window)
{
    windows_.push_back(std::move(window));
}

void IWindowVector::removeWindow(wid_t id)
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

IWindow* IWindowVector::getWindowById(wid_t id)
{
    for (auto& window : windows_)
    {
        if (window->getId() == id)
            return window.get();
    }

    return nullptr;
}

const IWindow* IWindowVector::getWindowById(wid_t id) const
{
    for (auto& window : windows_)
    {
        if (window->getId() == id)
            return window.get();
    }

    return nullptr;
}

}

