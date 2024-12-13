#ifndef FILTERSBAR_PLUGIN_HPP
#define FILTERSBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"


extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class FiltersBar : public ABar
{
public:
    FiltersBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released);

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class FiltersBarAction;
};

class FiltersBarAction : public AAction
{
public:
    FiltersBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FiltersBar* filters_bar);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    FiltersBar* filters_bar_;
};

class FiltersButton : public MenuSwitchButton
{
public:
    FiltersButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite, std::unique_ptr<psapi::IBar> menu);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;
private:

    friend class FiltersAction;
};

class FiltersAction : public AAction
{
public:
    FiltersAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FiltersButton* filters_button);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    FiltersButton* filters_button_ = nullptr;
};

static const psapi::wid_t kFiltersBarWindowId = 266;

#endif // FILTERSBAR_PLUGIN_HPP
