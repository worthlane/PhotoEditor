#include <cassert>
#include <iostream>

#include "plugins/brush.hpp"
#include "api/root_window.hpp"

static psapi::sfm::Texture btn;

static const psapi::sfm::IntRect BUTTON_RECT = {0, 0, 90, 90};

static const char* BUTTON_TEXTURE = "assets/textures/white.jpg";


bool loadPlugin()
{
    std::cout << "aue3\n";

    btn.loadFromFile(BUTTON_TEXTURE);

    psapi::sfm::Sprite btn_sprite;
    //btn_sprite.setTextureRect(BUTTON_RECT);
    //btn_sprite.setTexture(&btn);

    auto brush = std::make_unique<BrushButton>(psapi::vec2i(0, 0),
                                               psapi::vec2u(128, 32),
                                               std::make_unique<psapi::sfm::Sprite>(btn_sprite));


    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));

    if (tool_bar)
    {
        tool_bar->addWindow(std::move(brush));
    }
}

void unloadPlugin()
{
}


// ======================================================

BrushButton::BrushButton(const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite) :
    state_(BrushButton::State::Normal), size_(size), pos_(pos),
    sprite_(std::move(sprite)), parent_(nullptr), is_active_(true)
{
}

void BrushButton::setState(BrushButton::State state)
{
    state_ = state;
}

BrushButton::State BrushButton::getState() const
{
    return state_;
}

void BrushButton::draw(psapi::IRenderWindow* renderWindow)
{
    if (!is_active_)
        return;

    psapi::sfm::RenderWindow* desktop = static_cast<psapi::sfm::RenderWindow*>(renderWindow);

    sprite_->draw(desktop);
}

bool BrushButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!is_active_)
        return false;

    psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);

    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    bool is_hovered = (mouse_pos.x >= pos_.x) && (mouse_pos.x < pos_.x + size_.x) &&
                      (mouse_pos.y >= pos_.y) && (mouse_pos.y < pos_.y + size_.y);

    std::cout << "hovered: " << is_hovered << " LMB down: " << LMB_down << std::endl;


    switch (state_)
    {
        case BrushButton::State::Normal:

            if (is_hovered && LMB_down)
                state_ = BrushButton::State::Press;
            else if (is_hovered)
                state_ = BrushButton::State::Hover;
            break;

        case BrushButton::State::Press:

            if (is_hovered && !LMB_down)
                state_ = BrushButton::State::Released;
            else if (!is_hovered)
                state_ = BrushButton::State::Normal;
            break;

        case BrushButton::State::Released:

            if (is_hovered)
                state_ = BrushButton::State::Hover;
            break;
    }

    return true;
}

psapi::IWindow* BrushButton::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
};

const psapi::IWindow* BrushButton::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
};

psapi::vec2i BrushButton::getPos() const
{
    return pos_;
}

psapi::vec2u BrushButton::getSize() const
{
    return size_;
}

void BrushButton::setPos(const psapi::vec2i pos)
{
    pos_ = pos;
    sprite_->setPosition(pos.x, pos.y);
}

void BrushButton::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void BrushButton::forceActivate()
{
    is_active_ = true;
}

void BrushButton::forceDeactivate()
{
    is_active_ = false;
}

bool BrushButton::isActive() const
{
    return is_active_;
}

bool BrushButton::isWindowContainer() const
{
    return false;
};
