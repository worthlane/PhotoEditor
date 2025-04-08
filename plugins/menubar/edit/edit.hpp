#ifndef EDITBAR_PLUGIN_HPP
#define EDITBAR_PLUGIN_HPP

#include "api/api_photoshop.hpp"

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class UndoButton : public TextButton
{
public:
    UndoButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class UndoAction;
};

class UndoAction : public AAction
{
public:
    UndoAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, UndoButton* button);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

private:
    UndoButton* button_ = nullptr;
};

class RedoButton : public TextButton
{
public:
    RedoButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class RedoAction;
};

class RedoAction : public AAction
{
public:
    RedoAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, RedoButton* button);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

private:
    RedoButton* button_ = nullptr;
};

static const psapi::wid_t kRedoButtonId = 269;
static const psapi::wid_t kUndoButtonId = 270;

#endif // EDITBAR_PLUGIN_HPP
