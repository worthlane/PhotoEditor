#ifndef GEOMETRY_PLUGIN_HPP
#define GEOMETRY_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "standard/api_canvas.hpp"

#include "../plugins/toolbar_button.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class GeometryAction : public Action
{
public:
    GeometryAction(psapi::ICanvas* canvas, psapi::sfm::IShape* shape);
    ~GeometryAction() = default;

    virtual bool operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::sfm::IShape* shape_ = nullptr;
    psapi::ICanvas* canvas_    = nullptr;

    psapi::sfm::vec2i catch_pos_;
    psapi::sfm::IntRect shape_rect_;
};

static const psapi::wid_t kRectangleButtonId = 228;
//static const psapi::wid_t kEraserButtonId = 229;

#endif // GEOMETRY_PLUGIN_HPP
