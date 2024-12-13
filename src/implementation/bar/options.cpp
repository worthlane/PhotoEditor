#include <iostream>

#include "implementation/bar/options.hpp"
#include "style/design.hpp"

static const char* WHITE_TEXTURE = "assets/textures/white.jpg";
static const psapi::vec2u STD_SIZE = {76,60};

ColorPalette::ColorPalette()
{
    texture_ = psapi::sfm::ITexture::create();
    texture_.get()->loadFromFile(WHITE_TEXTURE);
    sprite_ = psapi::sfm::ISprite::create();
    make_styled_sprite(sprite_.get(), texture_.get(), {{0, 0}, STD_SIZE}, 0.3, {1, 2});
}

psapi::wid_t ColorPalette::getId() const
{
    return id_;
}

psapi::IWindow* ColorPalette::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
}

const psapi::IWindow* ColorPalette::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
}

psapi::sfm::vec2i ColorPalette::getPos() const
{
    return pos_;
}

psapi::sfm::vec2u ColorPalette::getSize() const
{
    return size_;
}

void ColorPalette::setSize(const psapi::sfm::vec2u& size)
{
    size_ = size;

    sprite_.get()->setTextureRect({{0,0}, {size.x, size.y}});
}

void ColorPalette::setPos(const psapi::sfm::vec2i& pos)
{
    pos_ = pos;

    sprite_.get()->setPosition(pos_.x, pos_.y);
}

void ColorPalette::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void ColorPalette::forceActivate()
{
    is_active_ = true;
}

void ColorPalette::forceDeactivate()
{
    is_active_ = false;
}

bool ColorPalette::isActive() const
{
    return is_active_;
}

bool ColorPalette::isWindowContainer() const
{
    return false;
}

psapi::sfm::Color ColorPalette::getColor() const
{
    return color_;
}

void ColorPalette::setColor(const psapi::sfm::Color &color)
{
    color_ = color;
    sprite_.get()->setColor(color);
}

void ColorPalette::draw(psapi::IRenderWindow* renderWindow)
{
    sprite_.get()->draw(renderWindow);
}

std::unique_ptr<psapi::IAction> ColorPalette::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<IdleAction>(renderWindow, event);
}

std::unique_ptr<psapi::IColorPalette> psapi::IColorPalette::create()
{
    return std::make_unique<ColorPalette>();
}

