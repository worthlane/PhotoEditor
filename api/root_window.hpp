#ifndef ROOT_WINDOW_HPP
#define ROOT_WINDOW_HPP

#include "api/window_vector.hpp"

namespace psapi
{

IWindowContainer* getRootWindow();

class RootWindow : public WindowVector
{
public:
    RootWindow() {};

    virtual wid_t getId() const override { return kRootWindowId; };

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual bool update(const IRenderWindow* renderWindow, const Event& event) override;

    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;
    virtual void setParent(const IWindow* parent) override;

    virtual void forceDeactivate() override;
    virtual void forceActivate() override;
    virtual bool isActive() const override;
};


}

#endif // ROOT_WINDOW_HPP
