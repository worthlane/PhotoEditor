#ifndef TOOLBAR_PLUGIN_HPP
#define TOOLBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class ToolBar : public ABar
{
public:
    ToolBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released);

    void draw(psapi::IRenderWindow* renderWindow) override;
    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class ToolBarAction;
};

class ToolBarAction : public AAction
{
public:
    ToolBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ToolBar* tool_bar);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;
private:
    ToolBar* tool_bar_;
};

#endif // TOOLBAR_PLUGIN_HPP
