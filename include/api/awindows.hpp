#ifndef PHOTOSHOP_HPP
#define PHOTOSHOP_HPP

#include "standard/api_photoshop.hpp"

namespace psapi {

class AWindow : public IWindow
{
public:
    virtual ~AWindow() = default;

    virtual wid_t getId() const override { return id_; };

protected:
    wid_t id_ = kInvalidWindowId;
};


class AWindowContainer : public IWindowContainer
{
public:

    virtual wid_t getId() const override { return id_; };

    virtual void addWindow(std::unique_ptr<IWindow> window) = 0;
    virtual void removeWindow(wid_t id) = 0;

    virtual bool isWindowContainer() const override { return true; };

protected:
    wid_t id_ = kInvalidWindowId;
};

class AWindowVector : public IWindowVector
{
public:
    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;

    virtual       IWindow* getWindowById(wid_t id)       override;
    virtual const IWindow* getWindowById(wid_t id) const override;

    virtual bool isWindowContainer() const override { return false; };

protected:
    std::vector<std::unique_ptr<IWindow> > windows_; ///< Vector of windows.

    wid_t id_ = kInvalidWindowId;
};

IWindowContainer* getRootWindow();

} // namespace psapi

#endif // PHOTOSHOP_HPP
