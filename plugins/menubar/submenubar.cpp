#include <string>
#include <iostream>

#include "../plugins/menubar/submenubar.hpp"

// ********************* SUBMENUBAR *********************/

SubMenuBar::SubMenuBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released) :
    ABar(id, pos, size, std::move(background),
                    std::move(normal),
                    std::move(hovered),
                    std::move(pressed),
                    std::move(released))
{
}


void SubMenuBar::draw(psapi::sfm::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    background_.get()->setPosition(pos_.x, pos_.y);
    background_.get()->draw(renderWindow);

    for (auto& button : buttons_)
    {
        finishButtonDraw(renderWindow, button.get());
        button.get()->draw(renderWindow);
    }

}
std::unique_ptr<psapi::IAction> SubMenuBar::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<SubMenuBarAction>(renderWindow, event, this);
}

// ******************** SUBMENUBAR ACTION ***********************

SubMenuBarAction::SubMenuBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, SubMenuBar* bar) :
AAction(render_window, event), bar_(bar)
{
}

bool SubMenuBarAction::execute(const Key& key)
{
    if (!bar_->is_active_)
        return false;

    bool flag = false;

    auto controller = psapi::getActionController();

    for (size_t i = 0; i < bar_->buttons_.size(); ++i)
    {
        auto& button = bar_->buttons_[i];

        psapi::IBarButton::State prev_state = button.get()->getState();

        flag |= controller->execute(button.get()->createAction(render_window_, event_));

        psapi::IBarButton::State cur_state = button.get()->getState();

        if (cur_state == psapi::IBarButton::State::Released &&
            prev_state != psapi::IBarButton::State::Released)
        {
            for (size_t j = 0; j < bar_->buttons_.size(); ++j)
                if (j != i) bar_->buttons_[j].get()->setState(psapi::IBarButton::State::Normal);
        }
    }

    return flag;
}

bool SubMenuBarAction::isUndoable(const Key& key)
{
    return false;
}

// ********************* CALLSUBMENUBUTTON *********************/

CallSubMenuButton::CallSubMenuButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color, std::unique_ptr<psapi::IBar> menu, const bool need_centering) :
                 TextMenuButton(id, bar, pos, size, name, color, std::move(menu), need_centering)
{}

std::unique_ptr<psapi::IAction> CallSubMenuButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
    {
        if (menu_->isActive())
            deactivateMenu();

        return std::make_unique<IdleAction>(renderWindow, event);
    }

    if (!menu_->isActive())
        activateMenu();

    return std::make_unique<CallSubMenuAction>(renderWindow, event, this);
}

CallSubMenuAction::CallSubMenuAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, CallSubMenuButton* button) :
                                AAction(render_window, event), button_(button)
{}

bool CallSubMenuAction::execute(const Key& key)
{
    auto controller = psapi::getActionController();
    controller->execute(button_->menu_->createAction(render_window_, event_));

    return true;
}

bool CallSubMenuAction::isUndoable(const Key& key)
{
    return false;
}
