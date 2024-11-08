#include <cassert>
#include <iostream>

#include "../plugins/toolbar_button.hpp"

// ======================================================

SwitchButton::SwitchButton(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite, psapi::ICanvas* canvas,
                         std::unique_ptr<Action> action) :
    id_(id), state_(SwitchButton::State::Normal), size_(size), pos_(pos),
    sprite_(std::move(sprite)), parent_(nullptr), is_active_(true), canvas_(canvas),
    action_(std::move(action))
{
    assert(action_);
}

psapi::wid_t SwitchButton::getId() const
{
    return id_;
}

void SwitchButton::setState(SwitchButton::State state)
{
    prev_state_ = state_;
    state_ = state;
}

SwitchButton::State SwitchButton::getState() const
{
    return state_;
}

void SwitchButton::draw(psapi::IRenderWindow* renderWindow)
{
    if (!is_active_)
        return;

    sprite_->setPosition(pos_.x, pos_.y);
    sprite_->draw(renderWindow);
}

bool SwitchButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!is_active_)
        return false;

    updateState(renderWindow, event);

    return (*(action_.get()))(renderWindow, event, canvas_);
}

void SwitchButton::updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);
    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    bool is_hovered = (mouse_pos.x >= pos_.x) && (mouse_pos.x < pos_.x + size_.x) &&
                      (mouse_pos.y >= pos_.y) && (mouse_pos.y < pos_.y + size_.y);

    switch (state_)
    {
        case SwitchButton::State::Normal:

            if (is_hovered)
                setState(SwitchButton::State::Hover);

            break;

        case SwitchButton::State::Hover:

            if (LMB_down)
                state_ = SwitchButton::State::Press;
            else if (!is_hovered)
                state_ = SwitchButton::State::Normal;

            break;

        case SwitchButton::State::Press:

            if (is_hovered && !LMB_down)
            {
                if (prev_state_ == SwitchButton::State::Normal)
                    state_ = SwitchButton::State::Released;
                else if (prev_state_ == SwitchButton::State::Released)
                    state_ = SwitchButton::State::Normal;
            }

            if (!is_hovered)
            {
                setState(prev_state_);
            }

            break;

        case SwitchButton::State::Released:

            if (prev_state_ == SwitchButton::State::Press)
            {
                if (!LMB_down)
                    prev_state_ = SwitchButton::State::Released;
            }
            else
            {
                if (is_hovered && LMB_down)
                    setState(SwitchButton::State::Press);
            }

            break;
    }
}

psapi::IWindow* SwitchButton::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
};

const psapi::IWindow* SwitchButton::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
};

psapi::vec2i SwitchButton::getPos() const
{
    return pos_;
}

psapi::vec2u SwitchButton::getSize() const
{
    return size_;
}

void SwitchButton::setPos(const psapi::vec2i pos)
{
    pos_ = pos;
    sprite_->setPosition(pos.x, pos.y);
}

void SwitchButton::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void SwitchButton::forceActivate()
{
    is_active_ = true;
}

void SwitchButton::forceDeactivate()
{
    is_active_ = false;
}

bool SwitchButton::isActive() const
{
    return is_active_;
}

bool SwitchButton::isWindowContainer() const
{
    return false;
};
