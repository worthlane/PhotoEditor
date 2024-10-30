#ifndef AWINDOWS_HPP
#define AWINDOWS_HPP

#include "standard/api_photoshop.hpp"

namespace psapi
{

class AWindow : public IWindow
{
public:
    virtual ~AWindow() = default;

    virtual wid_t getId() const override { return id_; };

    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;

protected:
    wid_t id_ = kInvalidWindowId;
};


class AWindowContainer : public IWindowContainer
{
public:

    virtual wid_t getId() const override { return id_; };

protected:
    wid_t id_ = kInvalidWindowId;
};

class AWindowVector : public IWindowVector
{
public:
    virtual ~AWindowVector() = default;

    virtual wid_t getId() const override { return id_; };

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;

    virtual       IWindow* getWindowById(wid_t id)       override;
    virtual const IWindow* getWindowById(wid_t id) const override;

protected:
    std::vector<std::unique_ptr<IWindow> > windows_; ///< Vector of windows.

    wid_t id_ = kInvalidWindowId;
};

IWindowContainer* getRootWindow();

class RootWindow : public AWindowVector
{
public:
    RootWindow() {};

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual bool update(const IRenderWindow* renderWindow, const Event& event) override;

    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;
    virtual void setParent(const IWindow* parent) override;

    virtual void forceDeactivate() override;
    virtual void forceActivate() override;
    virtual bool isActive() const override;

private:
    wid_t id_ = kRootWindowId;
};

} // namespace psapi

#endif // PHOTOSHOP_HPP
