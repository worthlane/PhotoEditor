#ifndef OPTIONBAR_PLUGIN_HPP
#define OPTIONBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class OptionBar : public AOptionsBar
{
public:
    OptionBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background);

    void draw(psapi::IRenderWindow* renderWindow) override;
    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class OptionBarAction;
};

class OptionBarAction : public AAction
{
public:
    OptionBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, OptionBar* option_bar);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;
private:
    OptionBar* option_bar_;
};


#endif // FILTERS_BAR_PLUGIN_HPP
