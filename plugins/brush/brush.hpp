#ifndef BRUSH_PLUGIN_HPP
#define BRUSH_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "standard/api_canvas.hpp"

#include "../plugins/bar_base/bar_button.hpp"
#include "../plugins/brush/catmull.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class PaintAction : public Action
{
public:
    PaintAction(const psapi::sfm::Color& color, const size_t radius, psapi::ICanvas* canvas, const bool scale_related = false);
    ~PaintAction() = default;

    virtual bool operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::sfm::Color color_;
    size_t            radius_;

    psapi::ICanvas* canvas_ = nullptr;

    bool scale_related_ = false;

    InterpolationArray array_;
};

static const psapi::wid_t kBrushButtonId = 228;
static const psapi::wid_t kEraserButtonId = 229;

#endif // BRUSH_PLUGIN_HPP
