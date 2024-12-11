#ifndef OPTIONBAR_PLUGIN_HPP
#define OPTIONBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {90, 90}};

class OptionBar : public ABar
{
public:
    OptionBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released);

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class OptionBarAction;
};

class OptionBarAction : public AAction
{
public:
    OptionBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, OptionBar* option_bar);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    OptionBar* option_bar_;
};


#endif // FILTERS_BAR_PLUGIN_HPP
