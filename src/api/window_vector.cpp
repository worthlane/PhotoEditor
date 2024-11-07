#include "api/window_vector.hpp"

namespace psapi
{

// ********************** WINDOW VECTOR *************************

void WindowVector::addWindow(std::unique_ptr<IWindow> window)
{
    windows_.push_back(std::move(window));
}

void WindowVector::removeWindow(wid_t id)
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

IWindow* WindowVector::getWindowById(wid_t id)
{
    for (auto& window : windows_)
    {
        if (window->getId() == id)
            return window.get();
    }

    return nullptr;
}

const IWindow* WindowVector::getWindowById(wid_t id) const
{
    for (auto& window : windows_)
    {
        if (window->getId() == id)
            return window.get();
    }

    return nullptr;
}

} // namespace psapi
