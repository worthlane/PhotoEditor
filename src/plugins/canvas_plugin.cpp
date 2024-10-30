#include <cassert>

#include "plugins/canvas_plugin.hpp"

using namespace psapi;

void Canvas::setParent(const IWindow* parent)
{
    return;
}

void Canvas::forceDeactivate()
{
    is_active_ = false;
}

void Canvas::forceActivate()
{
    is_active_ = true;
}

bool Canvas::isActive() const
{
    return is_active_;
}

IWindow* Canvas::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? static_cast<IWindow*>(this) : nullptr;
}
const IWindow* Canvas::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? static_cast<const IWindow*>(this) : nullptr;
}
