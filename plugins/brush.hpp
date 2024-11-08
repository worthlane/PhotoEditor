#ifndef BRUSH_PLUGIN_HPP
#define BRUSH_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "standard/api_canvas.hpp"

#include "../plugins/toolbar_button.hpp"
#include "../plugins/catmull.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class PaintAction : public Action
{
public:
    PaintAction(const psapi::sfm::Color& color, size_t radius);
    ~PaintAction() = default;

    virtual bool operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event,
                            psapi::ICanvas* canvas) override;

private:
    psapi::sfm::Color color_;
    size_t            radius_;

    InterpolationArray array_;
};

static const psapi::wid_t kBrushButtonId = 228;

#endif // BRUSH_PLUGIN_HPP
