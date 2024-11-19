#include <cassert>
#include <iostream>

#include "../plugins/toolbar_button.hpp"

// ======================================================

ABarButton::ABarButton(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite,
                         std::unique_ptr<Action> action) :
    id_(id), state_(ABarButton::State::Normal), size_(size), pos_(pos),
    sprite_(std::move(sprite)), parent_(nullptr), is_active_(true),
    action_(std::move(action))
{
    assert(action_);
}

psapi::wid_t ABarButton::getId() const
{
    return id_;
}

void ABarButton::setState(ABarButton::State state)
{
    state_ = state;
}

ABarButton::State ABarButton::getState() const
{
    return state_;
}

void ABarButton::draw(psapi::IRenderWindow* renderWindow)
{
    if (!is_active_)
        return;

    sprite_->setPosition(pos_.x, pos_.y);
    sprite_->draw(renderWindow);
}

psapi::IWindow* ABarButton::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
};

const psapi::IWindow* ABarButton::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
};

psapi::vec2i ABarButton::getPos() const
{
    return pos_;
}

psapi::vec2u ABarButton::getSize() const
{
    return size_;
}

void ABarButton::setPos(const psapi::vec2i pos)
{
    pos_ = pos;
    sprite_->setPosition(pos.x, pos.y);
}

void ABarButton::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void ABarButton::forceActivate()
{
    is_active_ = true;
}

void ABarButton::forceDeactivate()
{
    is_active_ = false;
}

bool ABarButton::isActive() const
{
    return is_active_;
}

bool ABarButton::isWindowContainer() const
{
    return false;
};

// ==================== SWITCH BUTTON =======================

SwitchButton::SwitchButton(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite,
                         std::unique_ptr<Action> action) :
    ABarButton(id, pos, size, std::move(sprite), std::move(action))
{
    assert(action_);
}

void SwitchButton::setState(ABarButton::State state)
{
    prev_state_ = state_;
    state_ = state;
}

bool SwitchButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return false;

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
        return false;

    return (*(action_.get()))(renderWindow, event);
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

// ====================== PRESS BUTTON =======================

PressButton::PressButton(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite,
                         std::unique_ptr<Action> action) :
    ABarButton(id, pos, size, std::move(sprite), std::move(action))
{
    assert(action_);
}

bool PressButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return false;

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return false;

    return (*(action_.get()))(renderWindow, event);
}

void PressButton::updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);
    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    bool is_hovered = (mouse_pos.x >= pos_.x) && (mouse_pos.x < pos_.x + size_.x) &&
                      (mouse_pos.y >= pos_.y) && (mouse_pos.y < pos_.y + size_.y);

    switch (state_)
    {
        case PressButton::State::Normal:

            if (is_hovered)
                setState(PressButton::State::Hover);

            break;

        case PressButton::State::Hover:

            if (LMB_down)
                setState(PressButton::State::Press);
            else if (!is_hovered)
                setState(PressButton::State::Normal);

            break;

        case PressButton::State::Press:

            if (is_hovered && !LMB_down)
                setState(PressButton::State::Released);
            else if (!is_hovered)
                setState(PressButton::State::Normal);

            break;

        case PressButton::State::Released:

            if (is_hovered)
                setState(PressButton::State::Hover);
            else
                setState(PressButton::State::Normal);

            break;
    }
}

