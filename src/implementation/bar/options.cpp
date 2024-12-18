#include <iostream>

#include "api/api_system.hpp"

#include "implementation/bar/options.hpp"
#include "style/design.hpp"


static const psapi::vec2u STD_SIZE = {200,28};

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
    pos_ = {0, 0};
    size_ = STD_SIZE;

    current_ = psapi::sfm::IRectangleShape::create(STD_SIZE);
    current_->setFillColor(color_);

    for (auto& color : BASE_COLORS)
    {
        auto choice = psapi::sfm::IRectangleShape::create(STD_SIZE);
        choice->setFillColor(color);
        variants_.push_back(std::move(choice));
    }

    placeColors(pos_, size_);
}

void ColorPalette::placeColors(const psapi::sfm::vec2i pos, const psapi::sfm::vec2u size)
{
    static const double CURR_SPACE_COEF = 0.6;

    current_->setSize(psapi::sfm::vec2u(size.y, size.y));
    current_->setPosition(pos);

    size_t side = size.y * CURR_SPACE_COEF;

    int gap = ((size.x - size.y) / BASE_COLORS.size()) - side;

    int y_pos  = pos.y + (size.y - side) / 2;
    int x_start = pos.x + gap + size.y;

    for (auto& variant : variants_)
    {
        variant->setSize(psapi::sfm::vec2u(side, side));
        variant->setPosition(psapi::sfm::vec2i(x_start, y_pos));
        x_start += side + gap;
    }

}

bool ColorPalette::hoverRectangle(const psapi::sfm::vec2i pos, const psapi::sfm::IRectangleShape* rect)
{
    auto rect_pos = rect->getPosition();
    auto rect_size = rect->getSize();

    return (pos.x >= rect_pos.x && pos.x <= rect_pos.x + rect_size.x &&
            pos.y >= rect_pos.y && pos.y <= rect_pos.y + rect_size.y);
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

    current_->setFillColor(color);
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
        if (palette_->hoverRectangle(mouse_pos, variant.get()))
        {
            palette_->setColor(variant->getFillColor());
            return true;
        }
    }

    return true;
}

bool ColorPaletteAction::isUndoable(const Key& key)
{
    return false;
}

