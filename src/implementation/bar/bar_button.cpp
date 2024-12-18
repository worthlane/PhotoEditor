#include <cassert>
#include <iostream>

#include "implementation/bar/bar_button.hpp"

static const char* MAIN_FONT = "assets/fonts/Helvetica_Neue_S.ttf";

// ======================================================

ABarButton::ABarButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite) :
    id_(id), state_(ABarButton::State::Normal), size_(size),
    sprite_(std::move(sprite)), parent_(bar), is_active_(true)
{
    setPos(pos);
}

psapi::wid_t ABarButton::getId() const
{
    return id_;
}

void ABarButton::setSize(const psapi::vec2u& size)
{
    size_ = size;
}

void ABarButton::setPos(const psapi::vec2i& pos)
{
    pos_ = pos + parent_->getPos();

    if (sprite_)
        sprite_->setPosition(pos_.x, pos_.y);
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

    //psapi::sfm::IntRect info = parent_->getNextChildInfo();

    //pos_ = info.pos;
    //size_ = {info.size.x, info.size.y};

    if (sprite_)
    {
        sprite_->setPosition(pos_.x, pos_.y);
        sprite_->draw(renderWindow);
    }

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

void ABarButton::setParent(const psapi::IWindow* parent)
{
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

SwitchButton::SwitchButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite) :
    ABarButton(id, bar, pos, size, std::move(sprite))
{
}

void SwitchButton::setState(ABarButton::State state)
{
    prev_state_ = state_;
    state_ = state;
}

/*bool SwitchButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return false;

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
        return false;

    return (*(action_.get()))(renderWindow, event);
}*/

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

PressButton::PressButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite) :
    ABarButton(id, bar, pos, size, std::move(sprite))
{
}

/*bool PressButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return false;

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return false;

    return (*(action_.get()))(renderWindow, event);
}*/

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

// =================== TEXT BUTTON =================

TextButton::TextButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color) :
                 PressButton(id, bar, pos, size, nullptr), name_(name), text_color_(color)
{
    font_ = psapi::sfm::IFont::create();
    font_->loadFromFile(MAIN_FONT);

    text_ = psapi::sfm::IText::create();
    text_->setFont(font_.get());

    text_->setFillColor(&text_color_);
    text_->setString(name);
    text_->setCharacterSize( 2.f/3.f * size_.y);
    text_gap_ = 1.f/3.f * size_.y;
}

void TextButton::draw(psapi::IRenderWindow* renderWindow)
{
    if (!is_active_)
        return;

    text_->setPos(psapi::sfm::vec2f(pos_.x + text_gap_, pos_.y));
    renderWindow->draw(text_.get());
}


// ****************** MENU BUTTON ************************

AMenuButton::AMenuButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite, std::unique_ptr<psapi::IBar> menu) :
    id_(id), state_(ABarButton::State::Normal), size_(size),
    sprite_(std::move(sprite)), parent_(bar), is_active_(true), menu_(std::move(menu))
{
    setPos(pos);
}

psapi::wid_t AMenuButton::getId() const
{
    return id_;
}

void AMenuButton::setSize(const psapi::vec2u& size)
{
    size_ = size;
}

void AMenuButton::setPos(const psapi::vec2i& pos)
{
    pos_ = pos + parent_->getPos();
    if (sprite_)
        sprite_->setPosition(pos_.x, pos_.y);
}

void AMenuButton::setState(AMenuButton::State state)
{
    state_ = state;
}

AMenuButton::State AMenuButton::getState() const
{
    return state_;
}

void AMenuButton::draw(psapi::IRenderWindow* renderWindow)
{
    if (!is_active_)
        return;

    //psapi::sfm::IntRect info = parent_->getNextChildInfo();

    //pos_ = info.pos;
    //size_ = {info.size.x, info.size.y};

    if (sprite_)
    {
        sprite_->setPosition(pos_.x, pos_.y);
        sprite_->draw(renderWindow);
    }

    menu_->draw(renderWindow);
}

psapi::IWindow* AMenuButton::getWindowById(psapi::wid_t id)
{
    auto found = menu_->getWindowById(id);

    if (!found)
        return found;

    return (id == getId()) ? this : nullptr;
};

const psapi::IWindow* AMenuButton::getWindowById(psapi::wid_t id) const
{
    auto found = menu_->getWindowById(id);

    if (!found)
        return found;

    return (id == getId()) ? this : nullptr;
};

psapi::vec2i AMenuButton::getPos() const
{
    return pos_;
}

psapi::vec2u AMenuButton::getSize() const
{
    return size_;
}

void AMenuButton::setParent(const psapi::IWindow* parent)
{
    parent_ = dynamic_cast<const psapi::IBar*>(parent);
}

void AMenuButton::forceActivate()
{
    is_active_ = true;
}

void AMenuButton::forceDeactivate()
{
    is_active_ = false;
}

bool AMenuButton::isActive() const
{
    return is_active_;
}

bool AMenuButton::isWindowContainer() const
{
    return false;
};

void AMenuButton::addMenuItem(std::unique_ptr<psapi::IWindow> item)
{
    menu_->addWindow(std::move(item));
}

void AMenuButton::activateMenu()
{
    menu_->forceActivate();
}
void AMenuButton::deactivateMenu()
{
    menu_->forceDeactivate();
}

psapi::IBar* AMenuButton::getMenu()
{
    return menu_.get();
}

const psapi::IBar* AMenuButton::getMenu() const
{
    return menu_.get();
}

// ************** MENU PRESS BUTTON ******************

MenuSwitchButton::MenuSwitchButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite, std::unique_ptr<psapi::IBar> menu) :
    AMenuButton(id, bar, pos, size, std::move(sprite), std::move(menu))
{
}

void MenuSwitchButton::setState(ABarButton::State state)
{
    prev_state_ = state_;
    state_ = state;
}

/*bool SwitchButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return false;

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
        return false;

    return (*(action_.get()))(renderWindow, event);
}*/

void MenuSwitchButton::updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
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

// ========= TEXT MENU BUTTON ==========

TextMenuButton::TextMenuButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                std::string& name, psapi::sfm::Color color, std::unique_ptr<psapi::IBar> menu, const bool need_centering) :
                MenuSwitchButton(id, bar, pos, size, nullptr, std::move(menu)), name_(name), text_color_(color)
{
    font_ = psapi::sfm::IFont::create();
    font_->loadFromFile(MAIN_FONT);

    text_ = psapi::sfm::IText::create();
    text_->setFont(font_.get());

    text_->setFillColor(&text_color_);
    text_->setString(name);

    size_t char_size = 2.f/3.f * size_.y;
    text_->setCharacterSize(char_size);

    static const double LEN_COEF = 0.35;

    if (need_centering)
        text_gap_ = (size.x - name.length() * char_size * LEN_COEF) / 2;
    else
        text_gap_ = 1.f/3.f * size_.y;
}

void TextMenuButton::draw(psapi::IRenderWindow* renderWindow)
{
    if (!is_active_)
        return;

    text_->setPos(psapi::sfm::vec2f(pos_.x + text_gap_, pos_.y));
    renderWindow->draw(text_.get());

    menu_->draw(renderWindow);
}
