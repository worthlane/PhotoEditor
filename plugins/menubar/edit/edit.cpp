#include <cassert>
#include <iostream>

#include "../plugins/menubar/edit/edit.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

bool onLoadPlugin()
{
    std::cout << "editbar loaded\n";

    auto root = psapi::getRootWindow();
    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto editbutton = static_cast<psapi::IMenuButton*>(root->getWindowById(psapi::kMenuToolsId));
    auto editbar = editbutton->getMenu();

    std::string undo_name = "Undo";
    auto undo = std::make_unique<UndoButton>(kUndoButtonId, editbar,
                                             psapi::vec2i(GAP.x / 2, GAP.y),
                                               psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                               undo_name, psapi::sfm::Color(255, 255, 255));

    std::string redo_name = "Redo";
    auto redo = std::make_unique<RedoButton>(kRedoButtonId, editbar,
                                               psapi::vec2i(GAP.x / 2, 2 * GAP.y + SUBBUTTON_RECT.size.y),
                                               psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                               redo_name, psapi::sfm::Color(255, 255, 255));

    expandMenu(editbar, undo.get());
    editbutton->addMenuItem(std::move(undo));

    expandMenu(editbar, redo.get());
    editbutton->addMenuItem(std::move(redo));

    return true;
}

void onUnloadPlugin()
{
}

// ======================================================

UndoButton::UndoButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                                std::string& name, psapi::sfm::Color color) :
                 TextButton(id, bar, pos, size, name, color)
{}

std::unique_ptr<psapi::IAction> UndoButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<UndoAction>(renderWindow, event, this);
}

UndoAction::UndoAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, UndoButton* button) :
                                AAction(render_window, event), button_(button)
{}

bool UndoAction::execute(const Key& key)
{
    auto controller = psapi::getActionController();

    controller->undo();

    return true;
}

bool UndoAction::isUndoable(const Key& key)
{
    return false;
}

RedoButton::RedoButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color) :
                 TextButton(id, bar, pos, size, name, color)
{}

std::unique_ptr<psapi::IAction> RedoButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<RedoAction>(renderWindow, event, this);
}

RedoAction::RedoAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, RedoButton* button) :
                                AAction(render_window, event), button_(button)
{}

bool RedoAction::execute(const Key& key)
{
    auto controller = psapi::getActionController();

    controller->redo();

    return true;
}

bool RedoAction::isUndoable(const Key& key)
{
    return false;
}



