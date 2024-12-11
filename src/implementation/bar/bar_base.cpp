#include <cassert>
#include <iostream>

#include "implementation/bar/bar_base.hpp"


psapi::wid_t ABar::getId() const
{
    return id_;
}

ABar::ABar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released) :
    id_(id), pos_(pos), size_(size),
    background_(std::move(background)),
    hovered_(std::move(hovered)),
    pressed_(std::move(pressed)),
    normal_(std::move(normal)),
    released_(std::move(released))
{
    parent_ = nullptr;
    is_active_ = true;
}

void ABar::addWindow(std::unique_ptr<psapi::IWindow> window)
{
    psapi::IBarButton* button = dynamic_cast<psapi::IBarButton*>(window.release());
    button->setParent(this);

    buttons_.push_back(std::unique_ptr<psapi::IBarButton>(button));
}

void ABar::removeWindow(psapi::wid_t id)
{
    for (size_t i = 0; i < buttons_.size(); ++i)
    {
        if (buttons_[i]->getId() == id)
        {
            buttons_.erase(buttons_.begin() + i);
            break;
        }
    }
}

void ABar::finishButtonDraw(psapi::sfm::IRenderWindow* renderWindow, const psapi::IBarButton* button) const
{
    psapi::vec2i pos = button->getPos();

    switch (button->getState())
    {
        case psapi::IBarButton::State::Normal:
            normal_.get()->setPosition(pos.x, pos.y);
            normal_.get()->draw(renderWindow);
            break;
        case psapi::IBarButton::State::Hover:
            hovered_.get()->setPosition(pos.x, pos.y);
            hovered_.get()->draw(renderWindow);
            break;
        case psapi::IBarButton::State::Press:
            pressed_.get()->setPosition(pos.x, pos.y);
            pressed_.get()->draw(renderWindow);
            break;
        case psapi::IBarButton::State::Released:
            released_.get()->setPosition(pos.x, pos.y);
            released_.get()->draw(renderWindow);
            break;
    }
}

psapi::IWindow* ABar::getWindowById(psapi::wid_t id)
{
    if (id == getId())
        return this;

    for (auto& button : buttons_)
    {
        if (button->getId() == id)
            return button.get();
    }

    return nullptr;
};

const psapi::IWindow* ABar::getWindowById(psapi::wid_t id) const
{
    if (id == getId())
        return this;

    for (auto& button : buttons_)
    {
        if (button->getId() == id)
            return button.get();
    }

    return nullptr;
};

psapi::vec2i ABar::getPos() const
{
    return pos_;
}
psapi::vec2u ABar::getSize() const
{
    return size_;
}

void ABar::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void ABar::forceActivate()
{
    is_active_ = true;
}
void ABar::forceDeactivate()
{
    is_active_ = false;
}
bool ABar::isActive() const
{
    return is_active_;
}

void ABar::setSize(const psapi::vec2u& size)
{
    size_ = size;
}

void ABar::setPos(const psapi::vec2i& pos)
{
    pos_ = pos;
}

bool ABar::unPressAllButtons()
{
    for (auto& button : buttons_)
    {
        auto state = button->getState();

        if (state == psapi::IBarButton::State::Released)
        {
            button->setState(psapi::IBarButton::State::Normal);
            return true;
        }
    }
    return false;
}

/*psapi::sfm::IntRect getNextChildInfo(const size_t curr_child, const psapi::sfm::vec2i& bar_pos)
{
    if (curr_child >= buttons_.size())
        return {{0, 0}, {0, 0}};

    psapi::vec2u size = buttons_[curr_child]->getSize();
    psapi::vec2i pos = buttons_[curr_child]->getPos();

    psapi::vec2i gap = size * 0.2;

    if (curr_child == 0)
        return {{bar_pos.x + gap.x, bar_pos.y + gap.y}, {size.x, size.y}};

    psapi::vec2u prev_size = buttons_[curr_child - 1]->getSize();
    psapi::vec2i prev_pos = buttons_[curr_child - 1]->getPos();

    psapi::sfm::IntRect info = {{prev_pos.x + prev_size.x + gap.x,  prev_pos.y},
                                {size.x, size.y}};

    if (info.pos.x + gap.x > bar_pos.x + size_.x)
    {
        info.pos = {bar_pos.x + gap.x, prev_pos.y + gap.y + prev_size.y};
    }

    return info;

}*/
