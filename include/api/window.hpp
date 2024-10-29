#ifndef PHOTOSHOP_HPP
#define PHOTOSHOP_HPP

#include "standard/api_photoshop.hpp"

namespace psapi {

class AWindow : public IWindow
{
public:
    virtual ~AWindow();

    virtual wid_t getId() const                 = 0;

    virtual vec2i getPos() const  = 0;
    virtual vec2u getSize() const = 0;

    virtual void setParent(const IWindow* parent) = 0;

    virtual void forceActivate()   = 0;
    virtual void forceDeactivate() = 0;
    virtual bool isActive() const  = 0;

    virtual bool isWindowContainer() const = 0;

private:
    wid_t id_ = kInvalidWindowId;
};


class IWindowContainer : public IWindow
{
public:

    virtual void addWindow(std::unique_ptr<IWindow> window) = 0;
    virtual void removeWindow(wid_t id) = 0;

    virtual bool isWindowContainer() const override;
};

class IWindowVector : public IWindowContainer
{
public:
    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;

    virtual       IWindow* getWindowById(wid_t id)       override;
    virtual const IWindow* getWindowById(wid_t id) const override;

    virtual bool isWindowContainer() const override;

protected:
    std::vector<std::unique_ptr<IWindow> > windows_; ///< Vector of windows.
};

IWindowContainer* getRootWindow();

using generalFunction = void* (*)(void*);
generalFunction getGeneralFunction(const std::string& name);

} // namespace psapi

#endif // PHOTOSHOP_HPP
