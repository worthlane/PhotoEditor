#ifndef AWINDOWS_HPP
#define AWINDOWS_HPP

#include "standard/api_photoshop.hpp"

namespace psapi
{

class WindowVector : public IWindowVector
{
public:
    virtual ~WindowVector() = default;

    virtual wid_t getId() const override { return kInvalidWindowId; };

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;

    virtual       IWindow* getWindowById(wid_t id)       override;
    virtual const IWindow* getWindowById(wid_t id) const override;
};

} // namespace psapi

#endif // AWINDOWS_HPP
