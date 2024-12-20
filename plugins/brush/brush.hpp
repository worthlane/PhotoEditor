#ifndef BRUSH_PLUGIN_HPP
#define BRUSH_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"
#include "api/api_memento.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

#include "../plugins/brush/catmull.hpp"

extern "C"
{

bool   onLoadPlugin();
void onUnloadPlugin();

}

class PaintButton : public SwitchButton
{
public:
    PaintButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 const size_t radius, const bool is_eraser = false);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::sfm::Color color_ = {255, 255, 255, 255};
    size_t radius_; // max_radius
    psapi::ICanvas* canvas_;

    psapi::IColorPalette* palette_ = nullptr;
    psapi::IOpacityOption* opacity_ = nullptr;
    psapi::IThicknessOption* thickness_ = nullptr;

    psapi::IOptionsBar* options_bar_;

    bool is_eraser_ = false;

    InterpolationArray array_;

    bool has_options_ = false;

    friend class PaintAction;
    friend class EraseAction;

    void replaceOptions();
    void createOptions();

    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> snapshots_;
    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> future_snapshots_;
};

class PaintAction : public AUndoableAction
{
public:
    PaintAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, PaintButton* button);

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    PaintButton* button_;
};

class EraseAction : public AUndoableAction
{
public:
    EraseAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, PaintButton* button);

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    PaintButton* button_;
};

static const psapi::wid_t kBrushButtonId = 228;
static const psapi::wid_t kEraserButtonId = 229;

#endif // BRUSH_PLUGIN_HPP
