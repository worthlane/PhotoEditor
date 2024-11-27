#ifndef GEOMETRY_PLUGIN_HPP
#define GEOMETRY_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "standard/api_canvas.hpp"

#include "../plugins/bar_base/bar_button.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class GeometryAction : public Action
{
public:
    GeometryAction(psapi::ICanvas* canvas, std::unique_ptr<psapi::sfm::IShape> shape);
    ~GeometryAction() = default;

    virtual bool operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    std::unique_ptr<psapi::sfm::IShape> shape_;
    psapi::ICanvas* canvas_    = nullptr;

    psapi::sfm::vec2i catch_pos_;
};

static const psapi::wid_t kRectangleButtonId = 228;
//static const psapi::wid_t kEraserButtonId = 229;

#endif // GEOMETRY_PLUGIN_HPP
