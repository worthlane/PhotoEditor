#ifndef BRUSH_PLUGIN_HPP
#define BRUSH_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

#include "../plugins/brush/catmull.hpp"

extern "C"
{

bool   onLoadPlugin();
void onUnloadPlugin();

}

class PaintAction : public AAction
{
public:
    PaintAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, const psapi::sfm::Color& color, const size_t radius, psapi::ICanvas* canvas);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    const psapi::sfm::Color& color_;
    size_t radius_;
    psapi::ICanvas* canvas_;

    InterpolationArray array_;
};

class PaintButton : public SwitchButton
{
public:
    PaintButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 const psapi::sfm::Color& color, const size_t radius, psapi::ICanvas* canvas);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    const psapi::sfm::Color& color_;
    size_t radius_;
    psapi::ICanvas* canvas_;
};

static const psapi::wid_t kBrushButtonId = 228;
static const psapi::wid_t kEraserButtonId = 229;

#endif // BRUSH_PLUGIN_HPP
