#ifndef FILTER_PLUGIN_HPP
#define FILTER_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class ContrastButton : public PressButton
{
public:
    ContrastButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 const double k, psapi::ICanvas* canvas);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    double k_;
    psapi::ICanvas* canvas_ = nullptr;

    friend class ContrastAction;
};

class ContrastAction : public AAction
{
public:
    ContrastAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ContrastButton* filter);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    ContrastButton* filter_ = nullptr;
};

class BlurButton : public PressButton
{
public:
    BlurButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 psapi::ICanvas* canvas);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    friend class BlurAction;
};

class BlurAction : public AAction
{
public:
    BlurAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, BlurButton* filter);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    BlurButton* filter_ = nullptr;
};

class SharpButton : public PressButton
{
public:
    SharpButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 psapi::ICanvas* canvas);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    friend class SharpAction;
};

class SharpAction : public AAction
{
public:
    SharpAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, SharpButton* filter);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    SharpButton* filter_ = nullptr;
};

class BareliefButton : public PressButton
{
public:
    BareliefButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 const double k, psapi::ICanvas* canvas);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    double k_;
    psapi::ICanvas* canvas_ = nullptr;

    friend class BareliefAction;
};

class BareliefAction : public AAction
{
public:
    BareliefAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, BareliefButton* filter);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    BareliefButton* filter_ = nullptr;
};

static const psapi::wid_t kNegativeFilterButtonId = 233;
static const psapi::wid_t kBareliefFilterButtonId = 234;
static const psapi::wid_t kBlurFilterButtonId = 235;
static const psapi::wid_t kSharpFilterButtonId = 236;

#endif // BRUSH_PLUGIN_HPP
