#include <iostream>

#include "api/api_system.hpp"

#include "implementation/bar/options.hpp"
#include "style/design.hpp"


static const char* WHITE_TEXTURE = "assets/textures/white.jpg";
static const psapi::vec2u STD_SIZE = {120,60};

using psapi::sfm::Color;

static const std::vector<psapi::sfm::Color> BASE_COLORS = {Color::getStandardColor(Color::Type::Black),
                                                           Color::getStandardColor(Color::Type::White),
                                                           Color::getStandardColor(Color::Type::Red),
                                                           Color::getStandardColor(Color::Type::Green),
                                                           Color::getStandardColor(Color::Type::Blue),
                                                           Color::getStandardColor(Color::Type::Yellow),
                                                           Color::getStandardColor(Color::Type::Magenta),
                                                           Color::getStandardColor(Color::Type::Cyan)};

ColorPalette::ColorPalette()
{
    curr_texture_ = psapi::sfm::ITexture::create();
    var_texture_ = psapi::sfm::ITexture::create();

    curr_texture_->loadFromFile(WHITE_TEXTURE);
    var_texture_->loadFromFile(WHITE_TEXTURE);

    pos_ = {0, 0};
    size_ = STD_SIZE;

    current_ = psapi::sfm::ISprite::create();
    make_styled_sprite(current_.get(), curr_texture_.get(), {{0, 0}, STD_SIZE}, 0.3, {1, 2});

    for (auto& color : BASE_COLORS)
    {
        auto choice = psapi::sfm::ISprite::create();
        //choice->setTextureRect({{0, 0}, STD_SIZE});
        //choice->setTexture(var_texture_.get());
        make_styled_sprite(choice.get(), var_texture_.get(), {{0, 0}, STD_SIZE}, 1, {0, 0});
        choice->setColor(color);
        variants_.push_back(std::move(choice));
    }

    placeColors(pos_, size_);
}

void ColorPalette::placeColors(const psapi::sfm::vec2i pos, const psapi::sfm::vec2u size)
{
    static const double CURR_SPACE_COEF = 0.6;

    current_->setTextureRect({{0,0}, {size.x, size.y * CURR_SPACE_COEF}});
    current_->setPosition(pos.x, pos.y);

    size_t height = size.y * (1 - CURR_SPACE_COEF);
    size_t length = size.x / variants_.size();
    int    y_pos  = pos.y + size.y - height;
    int    x_start = pos.x;

    for (auto& variant : variants_)
    {
        variant->setTextureRect({{0,0}, {length, height}});
        variant->setPosition(x_start, y_pos);
        x_start += length;
    }

}

bool ColorPalette::hoverSprite(const psapi::sfm::vec2i pos, const psapi::sfm::ISprite* sprite)
{
    auto sprite_pos = sprite->getPosition();
    auto sprite_size = sprite->getSize();

    return (pos.x >= sprite_pos.x && pos.x <= sprite_pos.x + sprite_size.x &&
            pos.y >= sprite_pos.y && pos.y <= sprite_pos.y + sprite_size.y);
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

    placeColors(pos_, size);
}

void ColorPalette::setPos(const psapi::sfm::vec2i& pos)
{
    pos_ = pos;

    placeColors(pos, size_);
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
    current_->setColor(color);
}

void ColorPalette::draw(psapi::IRenderWindow* renderWindow)
{
    current_->draw(renderWindow);

    for (auto& variant : variants_)
    {
        variant->draw(renderWindow);
    }
}

std::unique_ptr<psapi::IAction> ColorPalette::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<ColorPaletteAction>(renderWindow, event, this);
}

std::unique_ptr<psapi::IColorPalette> psapi::IColorPalette::create()
{
    return std::make_unique<ColorPalette>();
}


ColorPaletteAction::ColorPaletteAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ColorPalette* palette) :
            AAction(render_window, event), palette_(palette)
{}

bool ColorPaletteAction::execute(const Key& key)
{
    if (!psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left))
    {
        return false;
    }

    auto mouse_pos = psapi::sfm::Mouse::getPosition(render_window_);

    for (auto& variant : palette_->variants_)
    {
        if (palette_->hoverSprite(mouse_pos, variant.get()))
        {
            palette_->setColor(variant->getColor());
            return true;
        }
    }

    return true;
}

bool ColorPaletteAction::isUndoable(const Key& key)
{
    return false;
}

