#ifndef MENUBAR_PLUGIN_HPP
#define MENUBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class MenuBar : public ABar
{
public:
    MenuBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released);

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class MenuBarAction;
};

class MenuBarAction : public AAction
{
public:
    MenuBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, MenuBar* menu_bar);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    MenuBar* menu_bar_;
};

#endif // MENUBAR_PLUGIN_HPP
