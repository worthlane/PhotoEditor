#ifndef TOOLBAR_PLUGIN_HPP
#define TOOLBAR_PLUGIN_HPP

#include "api/bar.hpp"

class ToolBar : public IBar
{
public:

    virtual wid_t getId() const override { return kToolBarWindowId; };

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual bool update(const IRenderWindow* renderWindow, const Event& event) override;

    virtual ChildInfo getNextChildInfo() const override;
    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;
}

class ToolBarButton : public SimpleBarButton
{
public:
    virtual wid_t getId() const override { return kInvalidWindowId; };
}

#endif // TOOLBAR_PLUGIN_HPP
