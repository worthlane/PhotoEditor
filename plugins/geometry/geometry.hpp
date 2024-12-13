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

class GeometryButton : public SwitchButton
{
public:
    GeometryButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 std::unique_ptr<psapi::sfm::IShape> shape);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    std::unique_ptr<psapi::sfm::IShape> shape_;
    psapi::ICanvas* canvas_ = nullptr;
    psapi::IColorPalette* palette_ = nullptr;

    psapi::sfm::vec2i catch_pos_;

    psapi::IOptionsBar* options_bar_;
    std::vector<std::unique_ptr<psapi::IWindow>> options_;
    bool has_options_ = false;

    void replaceOptions();
    void createOptions();

    friend class GeometryAction;
};

class GeometryAction : public AAction
{
public:
    GeometryAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, GeometryButton* button);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    GeometryButton* button_;
};

static const psapi::wid_t kRectangleButtonId = 228;

#endif // GEOMETRY_PLUGIN_HPP
