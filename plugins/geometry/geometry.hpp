#ifndef GEOMETRY_PLUGIN_HPP
#define GEOMETRY_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class GeometryAction : public AAction
{
public:
    GeometryAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, psapi::sfm::IShape* shape, psapi::ICanvas* canvas, psapi::sfm::vec2i catch_pos);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    psapi::sfm::IShape* shape_ = nullptr;
    psapi::ICanvas* canvas_ = nullptr;

    psapi::sfm::vec2i catch_pos_;
};

class GeometryButton : public SwitchButton
{
public:
    GeometryButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 psapi::ICanvas* canvas, std::unique_ptr<psapi::sfm::IShape> shape);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    std::unique_ptr<psapi::sfm::IShape> shape_;
    psapi::ICanvas* canvas_ = nullptr;

    psapi::sfm::vec2i catch_pos_;
};

static const psapi::wid_t kRectangleButtonId = 228;

#endif // GEOMETRY_PLUGIN_HPP
