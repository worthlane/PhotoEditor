#ifndef SUBMENUBAR_PLUGIN_HPP
#define SUBMENUBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"

static const psapi::sfm::Color BACK_COLOR = psapi::sfm::Color(1, 1, 1);
static const psapi::sfm::Color RELEASE_COLOR = psapi::sfm::Color(2, 168, 167);
static const psapi::sfm::Color HOVER_COLOR = psapi::sfm::Color(2, 168, 167);
static const psapi::sfm::Color PRESS_COLOR = psapi::sfm::Color(2, 168, 167);
static const psapi::sfm::Color NORMAL_COLOR = psapi::sfm::Color(1, 1, 1);

static const psapi::sfm::IntRect SUBBUTTON_RECT = {{0, 0}, {268, 29}};
static const psapi::sfm::vec2i GAP = {2, 1};

void expandMenu(psapi::IBar* menu, psapi::IWindow* item);

class SubMenuBar : public ABar
{
public:
    SubMenuBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released);

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class SubMenuBarAction;
};

class SubMenuBarAction : public AAction
{
public:
    SubMenuBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, SubMenuBar* bar);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    SubMenuBar* bar_;
};

class CallSubMenuButton : public TextMenuButton
{
public:
    CallSubMenuButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                  std::string& name, psapi::sfm::Color color, std::unique_ptr<psapi::IBar> menu, const bool need_centering = true);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;
private:

    friend class CallSubMenuAction;
};

class CallSubMenuAction : public AAction
{
public:
    CallSubMenuAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, CallSubMenuButton* button);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    CallSubMenuButton* button_ = nullptr;
};

#endif // SUBMENUBAR_PLUGIN_HPP
