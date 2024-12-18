#ifndef FILTER_PLUGIN_HPP
#define FILTER_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

class NegativeButton : public TextButton
{
public:
    NegativeButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 const double k, psapi::ICanvas* canvas);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    double k_;
    psapi::ICanvas* canvas_ = nullptr;

    friend class NegativeAction;
};

class NegativeAction : public AAction
{
public:
    NegativeAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, NegativeButton* filter);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    NegativeButton* filter_ = nullptr;
};

class BlurButton : public TextButton
{
public:
    BlurButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
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

class ContrastButton : public TextButton
{
public:
    ContrastButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 psapi::ICanvas* canvas);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
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

class BareliefButton : public TextButton
{
public:
    BareliefButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
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
static const psapi::wid_t kContrastFilterButtonId = 236;

#endif // BRUSH_PLUGIN_HPP
