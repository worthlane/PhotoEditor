#ifndef CANVAS_PLUGIN_HPP
#define CANVAS_PLUGIN_HPP

#include "api/canvas.hpp"

class Canvas : public psapi::SimpleCanvas
{
public:
    Canvas(psapi::sfm::vec2i pos, psapi::sfm::vec2i size, psapi::sfm::vec2f scale) : SimpleCanvas(pos, size, scale) {}

    virtual psapi::wid_t getId() const override { return psapi::kCanvasWindowId; }

    virtual void setParent(const IWindow* parent) override;

    virtual void forceDeactivate() override;
    virtual void forceActivate() override;
    virtual bool isActive() const override;

    virtual IWindow* getWindowById(psapi::wid_t id) override;
    virtual const IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos() const override { return pos_; }
    virtual psapi::vec2u getSize() const override { return {size_.x, size_.y}; }

    virtual bool isWindowContainer() const override { return false; }

protected:
    bool is_active_ = true;
};


#endif // PLUGIN_EXAMPLE_HPP
